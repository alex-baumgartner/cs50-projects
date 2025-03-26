from datetime import datetime
from flask import redirect, session, render_template
from functools import wraps

def login_required(f):
    """
    Decorate routes to require login.

    http://flask.pocoo.org/docs/0.12/patterns/viewdecorators/
    """
    @wraps(f)
    def decorated_function(*args, **kwargs):
        if session.get("user_id") is None:
            return redirect("/login")
        return f(*args, **kwargs)
    return decorated_function

def apology(message, code=400):
    """Render message as an apology to user."""
    def escape(s):
        """
        Escape special characters.

        https://github.com/jacebrowning/memegen#special-characters
        """
        for old, new in [("-", "--"), (" ", "-"), ("_", "__"), ("?", "~q"),
                         ("%", "~p"), ("#", "~h"), ("/", "~s"), ("\"", "''")]:
            s = s.replace(old, new)
        return s
    return render_template("apology.html", top=code, bottom=escape(message)), code


def usd(value):
    """Format value as USD."""
    return f"${value:,.2f}"

def group_expenses_by_month(expenses):
    grouped_expenses = {}
    for expense in expenses:
        expense_date = datetime.strptime(expense["date"], '%Y-%m-%d')
        month = expense_date.strftime('%B %Y')
        if month not in grouped_expenses:
            grouped_expenses[month] = []
        grouped_expenses[month].append(expense)
    return grouped_expenses

def sort_expenses_by_month(grouped_expenses, reverse=False):
    sorted_expenses = dict(sorted(grouped_expenses.items(), key=lambda x: datetime.strptime(x[0], '%B %Y'), reverse=reverse))
    return sorted_expenses
