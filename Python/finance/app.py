import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from datetime import datetime
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# global variables
stock_balance_query = "SELECT SUM(share_amount) AS total_shares, stock_symbol AS symbol FROM transaction_history WHERE user_id = ? GROUP BY symbol HAVING total_shares > 0"

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

    user = db.execute("SELECT * FROM users WHERE id = ?", user_id)

    user_cash = user[0]["cash"]

    user_stocks = db.execute(stock_balance_query, user_id)

    total_stock_value = 0

    for stock in user_stocks:
        quote = lookup(stock["symbol"])
        stock["name"] = quote["name"]
        stock["price"] = quote["price"]
        stock["total_value"] = stock["total_shares"] * stock["price"]
        total_stock_value += stock["total_value"]

    total_user_value = user_cash + total_stock_value

    return render_template("index.html", user_stocks = user_stocks, cash = user_cash, total_user_value = total_user_value)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)
        symbol = request.form.get("symbol")

        if not request.form.get("shares"):
            return apology("must provide shares", 400)

        parameter = request.form.get("shares")

        if not parameter.isdigit():
            return apology("number of shares must not contain decimal places", 400)

        shares = int(request.form.get("shares"))

        if shares < 1:
            return apology("number of shares must be greater 0", 400)

        quote = lookup(symbol)

        if quote == None:
            return apology("symbol not found", 400)

        price = quote["price"]
        user_id = session["user_id"]

        users = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        user_data = users[0]

        available_user_funds = user_data["cash"]

        stock_payment = price * shares

        if available_user_funds < stock_payment:
            return apology("insufficient user funds", 400)

        transaction_timestamp = datetime.now()

        db.execute(
            "INSERT INTO transaction_history (user_id, stock_symbol, current_price, share_amount, transaction_timestamp) VALUES (?, ?, ?, ?, ?)",
            user_id, symbol, price, shares, transaction_timestamp)

        available_user_funds = user_data["cash"] - stock_payment

        db.execute("UPDATE users SET cash = ? WHERE id = ?", available_user_funds, user_id)

        return redirect("/")

    else:
        return render_template("buy.html")

@app.route("/deposit", methods=["GET", "POST"])
@login_required
def deposit():
    if request.method == "POST":
        if not request.form.get("depositType"):
            return apology("please select a deposit type", 403)
        if not request.form.get("amount"):
            return apology("please select an amount to transfer", 403)

        amount_to_add = int(request.form.get("amount"))

        if amount_to_add < 0:
            return apology("cannot add negative amount", 403)

        user_id = session["user_id"]

        user_data = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        current_cash = user_data[0]["cash"]

        updated_cash = current_cash + amount_to_add

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash, user_id)

        return redirect("/")

    else:
        return render_template("deposit.html")

@app.route("/history")
@login_required
def history():
    transaction_history = db.execute("SELECT * FROM transaction_history WHERE user_id = ?", session["user_id"])
    return render_template("history.html", transaction_history=transaction_history)


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
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    if request.method == "POST":

        if not request.form.get("symbol"):
            return apology("must provide stock symbol", 400)

        symbol = request.form.get("symbol")

        quote = lookup(symbol)

        if quote == None:
            return apology("symbol not found", 400)

        name = quote["name"]
        price = quote["price"]

        return render_template("quoted.html", name=name, symbol=symbol, price=price)

    else:
        return render_template("quote.html")


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

        db.execute("INSERT INTO users(username, hash) VALUES(?, ?)", username, generate_password_hash(password))
        return redirect("/")

    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    user_id = session["user_id"]

    symbols_information = db.execute(stock_balance_query, user_id)

    symbols = []

    for row in symbols_information:
        symbols.append(row["symbol"])

    if request.method == "POST":
        if not request.form.get("symbol"):
            return apology("please select a valid symbol!", 400)

        if not request.form.get("shares"):
            return apology("must provide shares", 400)
        shares_to_sell = int(request.form.get("shares"))

        if shares_to_sell < 0:
            return apology("number of shares to sell must be positive", 400)

        selected_symbol = request.form.get("symbol")

        symbol_information = db.execute("SELECT SUM(share_amount) AS total_shares FROM transaction_history WHERE user_id = ? AND stock_symbol = ?", user_id , selected_symbol)

        if symbol_information[0]["total_shares"] < shares_to_sell:
            return apology("cannot sell more shares than available", 400)

        current_timestamp = datetime.now()

        quote = lookup(selected_symbol)

        if quote == None:
            return apology("symbol not found", 400)

        price = quote["price"]

        db.execute("INSERT INTO transaction_history(user_id, stock_symbol, current_price, share_amount, transaction_timestamp) VALUES (?, ?, ?, ?, ?)",
                   user_id, selected_symbol, price, -shares_to_sell, current_timestamp)

        additional_cash = shares_to_sell * price

        user_information = db.execute("SELECT * FROM users WHERE id = ?", user_id)

        updated_cash_balance = user_information[0]["cash"] + additional_cash

        db.execute("UPDATE users SET cash = ? WHERE id = ?", updated_cash_balance, user_id)

        return redirect("/")

    else:
        return render_template("sell.html", symbols=symbols)
