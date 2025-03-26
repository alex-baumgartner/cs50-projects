import os
import matplotlib.pyplot as plt
import numpy as np

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from datetime import datetime
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import login_required, usd, group_expenses_by_month, sort_expenses_by_month, apology

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///expensetracker.db")

# global variables
months_dict = {
    '01': 'JAN',
    '02': 'FEB',
    '03': 'MAR',
    '04': 'APR',
    '05': 'MAY',
    '06': 'JUN',
    '07': 'JUL',
    '08': 'AUG',
    '09': 'SEP',
    '10': 'OCT',
    '11': 'NOV',
    '12': 'DEC'
}


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

@app.route("/")
@login_required
def index():
    user_id = session["user_id"]
    expenses = db.execute("SELECT e.id, date, name AS category, amount, description FROM expenses e JOIN categories c ON c.id = e.category_id WHERE user_id = ?", user_id)
    grouped_expenses = group_expenses_by_month(expenses)
    display_expenses_descending = True
    sorted_expenses = sort_expenses_by_month(grouped_expenses, display_expenses_descending)
    budget_information = db.execute("SELECT * FROM budgets WHERE user_id = ?", user_id)
    budget = budget_information[0]["amount"]
    return render_template("index.html", expenses=sorted_expenses, budget=budget)

@app.route("/budget", methods=["GET", "POST"])
@login_required
def budget():
    """Set a budget for the user that is logged in"""
    if request.method == "POST":
        if not request.form.get("budget"):
            return apology("must provide budget", 403)
        budget = float(request.form.get("budget"))
        if budget <= 0:
            return apology("configured budget must be greater than 0", 403)
        user_id = session["user_id"]
        current_budget = db.execute("SELECT * FROM budgets WHERE user_id = ?", user_id)
        if len(current_budget) > 0:
            db.execute("UPDATE budgets SET amount = ? WHERE user_id = ?", budget, user_id)
        else:
            db.execute("INSERT INTO budgets(user_id, amount) VALUES (?, ?)", user_id, budget)
        return redirect("/")

    else:
        return render_template("budget.html")

@app.route("/chart")
@login_required
def chart():
    user_id = session["user_id"]
    budget_db_data = db.execute("SELECT * FROM budgets WHERE user_id = ?", user_id)
    budget = budget_db_data[0]["amount"]
    monthly_expenses = get_year_of_monthly_expenses(user_id)
    months = []
    expense_data = []
    budget_data = []

    for month_number, amount in monthly_expenses.items():
        months.append(months_dict[month_number])
        expense_data.append(amount)
        budget_data.append(budget)

    plt.figure(figsize=(15, 6))
    plt.grid(True)
    plt.grid(True, linestyle='--', linewidth=0.5, alpha=0.7, color='gray')

    trendline = create_trendline(expense_data)
    plt.plot(months, trendline, label="Trend")

    plt.plot(months, budget_data, label="Budget")
    plt.plot(months, expense_data, label="Expenses")

    plt.gca().set_facecolor("lightgray")
    plt.xlabel("Months")
    plt.ylabel("Amount")
    plt.title("Monthly Expenses vs. Budget")
    plt.legend()

    chart_filename = "static/chart.jpg"
    plt.savefig(chart_filename)

    return render_template("chart.html", chart_filename=chart_filename)

@app.route("/expense-add", methods=["GET", "POST"])
@login_required
def expenseAdd():
    user_id = session["user_id"]
    if request.method == "POST":
        if not request.form.get("category"):
            return apology("please select a category", 403)
        category = request.form.get("category")
        if not request.form.get("date"):
            return apology("please select a valid date", 403)
        date = request.form.get("date")
        if not request.form.get("amount"):
            return apology("please enter a valid number for the expense amount", 403)
        amount = float(request.form.get("amount"))
        if amount < 0:
            return apology("amount of expense must be positive", 403)
        description = ""
        if request.form.get("description"):
            description = request.form.get("description")
            db.execute("INSERT INTO expenses(user_id, date, category_id, amount, description) VALUES (?, ?, ?, ?, ?)", user_id, date, category, amount, description)
        return redirect("/")
    else:
        currentDate = datetime.now()
        categories = get_categories()
        return render_template("expense-add.html", currentDate=currentDate, categories=categories)

@app.route("/expense-delete/<int:expense_id>", methods=["POST"])
@login_required
def expense_delete(expense_id):
    user_id = session["user_id"]
    expense = db.execute("SELECT * FROM expenses WHERE id = ? AND user_id = ?", expense_id, user_id)

    if not expense:
        return apology("expense not found", 403)

    db.execute("DELETE FROM expenses WHERE id = ?", expense_id)

    return redirect("/")

@app.route("/expense-edit/<int:expense_id>", methods=["GET", "POST"])
@login_required
def expense_edit(expense_id):
    user_id = session["user_id"]
    expense_data = db.execute("SELECT e.*, c.name AS category FROM expenses e JOIN categories c ON e.category_id = c.id WHERE e.id = ? AND e.user_id = ?", expense_id, user_id)
    expense = expense_data[0]
    if not expense:
        return apology("expense not found", 403)
    if request.method == "POST":
        expense_date = expense["date"]
        expense_amount = expense["amount"]
        expense_category = expense["category_id"]
        expense_description = expense["description"]

        changes = False

        if request.form.get("date"):
            expense_date = request.form.get("date")
            changes = True
        if request.form.get("amount"):
            expense_amount = float(request.form.get("amount"))
            if expense_amount <= 0:
                return apology("amount of expense must be positive", 403)
            changes = True
        if request.form.get("category"):
            expense_category = request.form.get("category")
            changes = True
        if request.form.get("description"):
            expense_description = request.form.get("description")
            changes = True

        if changes:
            db.execute("UPDATE expenses SET date = ?, amount = ?, category_id = ?, description = ? WHERE id = ?", expense_date, expense_amount, expense_category, expense_description, expense_id)

        return redirect("/")

    else:
        categories = get_categories()
        return render_template("expense-edit.html", expense=expense, categories=categories)

@app.route("/filter-expenses", methods=["GET", "POST"])
@login_required
def filterExpenses():
    if request.method == "POST":
        dateFrom = None
        dateTo = None
        category = None

        if request.form.get("dateFrom"):
            dateFrom = request.form.get("dateFrom")
        if request.form.get("dateTo"):
            dateTo = request.form.get("dateTo")
        if dateFrom and dateTo and dateFrom > dateTo:
            return apology("start date must be smaller than end date!", 403)
        if request.form.get("category"):
            category = request.form.get("category")

        if not dateFrom and not dateTo and not category:
            return apology("please select at least one filter option!", 403)

        query = "SELECT e.id, date, name AS category, amount, description FROM expenses e JOIN categories c ON c.id = e.category_id WHERE e.user_id = ?"
        clauses = []
        values = []

        user_id = session["user_id"]
        values.append(user_id)

        if dateFrom:
            clauses.append("date > ?")
            values.append(dateFrom)
        if dateTo:
            clauses.append("date < ?")
            values.append(dateTo)
        if category:
            clauses.append("category_id = ?")
            values.append(category)

        if clauses:
            query = query + " AND " + " AND ".join(clauses)

        expenses = db.execute(query, *values)
        grouped_expenses = group_expenses_by_month(expenses)
        filtered_expenses = sort_expenses_by_month(grouped_expenses)

        return render_template("filtered-expenses.html", filtered_expenses=filtered_expenses)

    else:
        categories = get_categories()
        return render_template("filter-expenses.html", categories=categories)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id

    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = ?", request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["password"], request.form.get("password")):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    if request.method == "POST":
        """Register user"""

        if not request.form.get("username"):
            return apology("must provide username", 400)

        username = request.form.get("username")

        if not request.form.get("password"):
            return apology("must provide password", 400)

        password = request.form.get("password")

        if not request.form.get("confirmation") or request.form.get("confirmation") != password:
            return apology("confirmation must match password", 400)

        existing_user = db.execute("SELECT * FROM users WHERE username = ?", username)

        if len(existing_user) > 0:
            return apology("user already exists!", 400)

        db.execute("INSERT INTO users(username, password) VALUES(?, ?)", username, generate_password_hash(password))
        return redirect("/")

    else:
        return render_template("register.html")

@app.route("/statistics", methods=["GET"])
@login_required
def statistics():
    user_id = session["user_id"]

    # Calculate top categories
    top_categories = db.execute("SELECT c.name AS category, SUM(amount) AS total_amount FROM expenses e JOIN categories c ON e.category_id = c.id WHERE user_id = ? GROUP BY category_id ORDER BY total_amount DESC LIMIT 3", user_id)

    # Calculate top months
    top_months = db.execute("SELECT strftime('%Y-%m', date) AS month, SUM(amount) AS total_amount FROM expenses WHERE user_id = ? GROUP BY month ORDER BY total_amount DESC LIMIT 3", user_id)

    return render_template("statistics.html", top_categories=top_categories, top_months=top_months)



def get_categories():
    categories = db.execute("SELECT * FROM categories")
    if len(categories) <= 0:
        return apology("no categories existing", 403)
    return categories

def get_year_of_monthly_expenses(user_id):
    expenses = db.execute("SELECT expenses.amount, strftime('%m', expenses.date) AS month_number FROM expenses WHERE user_id = ? AND strftime('%Y-%m', expenses.date) <= strftime('%Y-%m', date('now', '-1 month', 'start of month')) AND strftime('%Y-%m', expenses.date) >= strftime('%Y-%m', date('now', '-12 month', 'start of month')) ORDER BY expenses.date", user_id)

    monthly_expenses = {}
    for expense in expenses:
        if expense["month_number"] not in monthly_expenses:
            monthly_expenses[expense["month_number"]] = expense["amount"]
        else:
            monthly_expenses[expense["month_number"]] += expense["amount"]
    return monthly_expenses

def create_trendline(data):
    trendline_coefficients = np.polyfit(range(len(data)), data, 1)
    return np.polyval(trendline_coefficients, range(len(data)))
