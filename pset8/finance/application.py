import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Get user stock information
    stocks = db.execute("SELECT symbol, name, SUM(quantity) AS shares FROM transactions WHERE userid IS :userid GROUP BY symbol", userid=session["user_id"])
    # Store the account total
    total = 0.00
    # For each stock owned, get its current value and value for all stocks owned
    for stock in stocks:
        stock["price"] = usd(lookup(stock["symbol"])["price"])
        stock["total"] = usd(lookup(stock["symbol"])["price"] * float(stock["shares"]))
        total = total + lookup(stock["symbol"])["price"] * float(stock["shares"])
    cash = db.execute("SELECT cash FROM users WHERE id IS :userid", userid=session["user_id"])[0]["cash"]
    total = total + float(cash)
    return render_template("index.html", stocks=stocks, cash=usd(cash), total=usd(total))

@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "POST":

        # Get stock information
        stock = lookup(request.form.get("symbol"))

        # If it returns a valid stock
        if stock:
            # Must be positive qty
            if int(request.form.get("qty")) > 0:
                # Verify the funds are there
                if stock["price"] * float(request.form.get("qty")) < db.execute("SELECT * FROM users WHERE id = :userid",
                                                                                                                userid=session["user_id"])[0]["cash"]:
                    # Record the transaction
                    db.execute("INSERT INTO transactions (userid, symbol, price, quantity, name) VALUES (?, ?, ?, ?, ?)",
                              session["user_id"], stock["symbol"], stock["price"], request.form.get("qty"), stock["name"])

                    # Remove the funds
                    db.execute("UPDATE users SET cash = cash - :price * :qty WHERE id IS :id",
                                price=stock["price"], qty=float(request.form.get("qty")), id=session["user_id"])

                    # Redirect user to home page
                    return redirect("/")
                # not enough money for transaction
                else:
                    return apology("insufficient funds", 400)
            # Qty is less then 1
            else:
                return apology("Must be positive qty", 400)
        # Symbol not recognized
        else:
            return apology("invalid symbol", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    history = db.execute("SELECT symbol, quantity, price, timestamp FROM transactions WHERE userid IS :userid", userid=session["user_id"])
    return render_template("sell.html", history=history)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("invalid username and/or password", 400)

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
    """Get stock quote."""
    if request.method == "POST":

        # Get stock information
        stock = lookup(request.form.get("symbol"))

        # If it returns a valid stock, show its information
        if stock:
            return render_template("quoted.html", symbol=stock["symbol"], name=stock["name"], price=usd(stock["price"]))
        else:
            return apology("invalid symbol", 400)

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
# User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 400)

        # Ensure passwords match
        elif request.form.get("password") != request.form.get("confirmation"):
            return apology("passwords don't match", 400)

        # Ensure the username is not already taken
        if len(db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))) > 0:
            return apology("user already exists", 400)

        # Query database for username
        db.execute("INSERT INTO users (username, hash) VALUES (?, ?)",
                          request.form.get("username"), generate_password_hash(request.form.get("password")))

        # Query database for username to login
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1:
            return apology("multiple users found", 400)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    if request.method == "POST":
        # Check they they have enough shares to trade
        if db.execute('SELECT SUM(quantity) AS shares FROM transactions WHERE (userid IS :userid) AND (symbol IS :symbol)', userid=session["user_id"],
                       symbol=request.form.get("symbol"))[0]["shares"] >= int(request.form.get("qty")):
            # If they do make the sale
            stock = lookup(request.form.get("symbol"))
            # Record the transaction
            db.execute("INSERT INTO transactions (userid, symbol, price, quantity, name) VALUES (?, ?, ?, ?, ?)",
                        session["user_id"], stock["symbol"], stock["price"], 0-int(request.form.get("qty")), stock["name"])
            # Give the money
            db.execute("UPDATE users SET cash = cash + :price * :qty WHERE id IS :id",
                                price=stock["price"], qty=float(request.form.get("qty")), id=session["user_id"])
        else:
            return apology("too many shares", 400)
        return redirect("/")
    else:
        # Get user stock info
        stocks = db.execute("SELECT symbol, SUM(quantity) AS shares FROM transactions WHERE userid IS :userid GROUP BY symbol", userid=session["user_id"])
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
