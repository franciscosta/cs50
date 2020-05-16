import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# -------------------------------------------

# Configure application
app = Flask(__name__)

# -------------------------------------------

# Ensure templates are auto-reloaded

app.config["TEMPLATES_AUTO_RELOAD"] = True

# -------------------------------------------

# Ensure responses aren't cached


@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# -------------------------------------------

# Custom filter


app.jinja_env.filters["usd"] = usd

# -------------------------------------------

# Configure session to use filesystem (instead of signed cookies)

app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# -------------------------------------------

# Configure CS50 Library to use SQLite database

db = SQL("sqlite:///finance.db")

# -------------------------------------------

# Make sure API key is set

if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


# -------------------------------------------

# Small function to get the username

def getusername(value):

    # Get the user's username
    usersql = db.execute("SELECT username FROM users WHERE id = :value", value=value)

    # Convert username into a string
    temp = usersql[0]
    user = temp["username"]

    return user

# -------------------------------------------


@app.route("/")
@login_required
def index():

    # Get the username
    user = getusername(session["user_id"])

    # Get all transactions
    portfolio = db.execute(
        "SELECT symbol, SUM(shares) as totals FROM transactions WHERE buyer = :user GROUP BY symbol HAVING totals > 0;", user=user)

    totalvalue = 0

    for row in portfolio:
        info = lookup(row["symbol"])
        row.update(info)
        totalvalue = totalvalue + (float(row["totals"]) * float(row["price"]))

    # Cash + Total
    currentcash = db.execute("SELECT cash FROM users WHERE username = :user", user=user)

    totalvalue = totalvalue + float(currentcash[0]["cash"])

    # Render Template
    return render_template("index.html", portfolio=portfolio, currentcash=currentcash, totalvalue=totalvalue)


# -------------------------------------------


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():

    if request.method == "POST":

        # Making sure all info is correct
        if not request.form.get("symbol"):
            flash("Please select a symbol")
            return redirect("/buy")

        if not request.form.get("shares"):
            flash("You didn't enter any shares")
            return redirect("/buy")

        if int(request.form.get("shares")) < 1:
            flash("Enter at least one share")
            return redirect("/buy")

        if lookup(request.form.get("symbol")) == None:
            flash("Invalid symbol")
            return redirect("/buy")

        # --------------------------------------

        # Variables
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        # Get the price of the stock
        info = lookup(symbol)
        price = info["price"]

        # Total cost
        cost = float(shares) * price

        # Can the user afford it?
        currentcash = db.execute("SELECT cash FROM users WHERE id = :userid", userid=session["user_id"])

        # Conversiting the result of the query into usable material
        temp = currentcash[0]

        # How much money is the user left with
        cashleft = float(temp["cash"]) - cost

        # Get userid
        userid = session["user_id"]

        # Get username
        user = getusername(session["user_id"])

        # If not enough cash
        if cost > float(temp["cash"]):
            flash("You don't have enough funds")
            return redirect("/buy")

        # If enough cash
        else:
            buy = "buy"
            db.execute("INSERT INTO transactions (symbol, shares, cost, buyer, type) VALUES (:symbol, :shares, :price, :user, :buy)",
                       symbol=symbol, shares=shares, price=price, user=user, buy=buy)
            db.execute("UPDATE users SET cash = :cashleft WHERE id = :userid", cashleft=cashleft, userid=userid)

            flash("Bought!")

            return redirect("/")

    else:
        return render_template("buy.html")


# -------------------------------------------


@app.route("/history")
@login_required
def history():

    # Get the username
    user = getusername(session["user_id"])

    # Get all transactions
    transactions = db.execute("SELECT * FROM transactions WHERE buyer = :user", user=user)

    return render_template("history.html", t=transactions)


# -------------------------------------------


@app.route("/login", methods=["GET", "POST"])
def login():

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("Must provide a username", 403)

        # Ensure password was submitted
        if not request.form.get("password"):
            return apology("Must provide a username", 403)

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
            return apology("Invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        flash("Logged in!")

        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


# -------------------------------------------


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    flash("Logged out!")

    # Redirect user to login form
    return redirect("/")


# -------------------------------------------


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():

    # If the user requests a quote
    if request.method == "POST":

        # If the user doesn't enter a symbol
        if not request.form.get("symbol"):
            flash("Must provide a symbol")
            return redirect("/quote")

        # Lookup the price of the symbol
        quote = lookup(request.form.get("symbol"))

        # If the symbol is invalid
        if quote == None:
            flash("Must provide a valid symbol")
            return redirect("/quote")

        # If the symbol is valid
        else:
            return render_template("quote_result.html", quote=quote)

    else:
        return render_template("quote.html")


# -------------------------------------------


@app.route("/register", methods=["GET", "POST"])
def register():

    # If its a POST request
    if request.method == "POST":

        # Take care of all mistakes
        if not request.form.get("username"):
            flash("Must provide username")
            return redirect("/register")

        elif not request.form.get("password"):
            flash("Must provide password")
            return redirect("/register")

        elif not request.form.get("confirm_password"):
            flash("Must confirm password")
            return redirect("/register")

        elif request.form.get("password") != request.form.get("confirm_password"):
            flash("Passwords don't match password")
            return redirect("/register")

        # --------------------------------------

        # Query DB for a matching username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        if len(rows) > 0:
            flash("Username is taken")
            return redirect("/register")

        # --------------------------------------

        # Store my credentials into variables
        u = request.form.get("username")
        h = generate_password_hash(request.form.get("password"))

        # Store them in the DB
        new_user = db.execute("INSERT INTO users (username, hash) VALUES (?, ?)", u, h)

        flash("Registered!")

        return redirect("/login")

    # If its a GET request
    else:
        return render_template("register.html")


# -------------------------------------------


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():

    # Get the username
    user = getusername(session["user_id"])

    # Create a list of all the stocks I currently own
    stocks = db.execute("SELECT symbol FROM transactions WHERE buyer = :user GROUP BY symbol HAVING SUM(shares) > 0;", user=user)

    if request.method == "POST":

        # Making sure all info is correct
        if not request.form.get("symbol"):
            flash("Please select a symbol")
            return redirect("/sell")

        if not request.form.get("shares"):
            flash("You didn't enter any shares")
            return redirect("/sell")

        if int(request.form.get("shares")) < 1:
            flash("Enter at least one share")
            return redirect("/sell")

        # ----------------------------------------------

        # Get the symbol and the shares
        symbol = request.form.get("symbol")
        shares = int(request.form.get("shares"))

        # Request their current price
        quote = lookup(symbol)
        price = quote["price"]

        # Get how many stocks the user currently owns
        portfolio = db.execute(
            "SELECT symbol, SUM(shares) as totals FROM transactions WHERE buyer = :user AND symbol = :symbol GROUP BY symbol HAVING totals > 0;", user=user, symbol=symbol)

        # If the user doesn't own that many stocks
        if int(shares) > portfolio[0]["totals"]:
            flash("You don't own that many shares")
            return redirect("/sell")

        # If the user owns that many stocks
        else:

            # How much money is the user going to make
            cashback = price * int(shares)

            # How much cash does the user currently have
            cashrequest = db.execute("SELECT cash FROM users WHERE username = :user", user=user)
            cashavail = float(cashrequest[0]["cash"])

            # Sum those two
            totalcash = cashback + cashavail

            # Making shares a negative
            shares = shares * -1

            # Setting type of transaction
            t = "sell"

            # Changes to the database
            db.execute("INSERT INTO transactions (symbol, shares, cost, buyer, type) VALUES (:symbol, :shares, :price, :user, :t)",
                       symbol=symbol, shares=shares, price=price, user=user, t=t)
            db.execute("UPDATE users SET cash = :totalcash WHERE username = :user", totalcash=totalcash, user=user)

            flash("The sale was successful")

            return redirect("/")

    else:

        return render_template("sell.html", stocks=stocks)


# -------------------------------------------

@app.route("/changepassword", methods=["GET", "POST"])
@login_required
def changePassword():

    if request.method == "POST":

        # Error checking
        if not request.form.get("current_password"):
            flash("You must enter your current password")
            return redirect("/changepassword")

        if not request.form.get("new_password"):
            flash("You must enter your new password")
            return redirect("/changepassword")

        if not request.form.get("confirm_password"):
            flash("You must confirm your new password")
            return redirect("/changepassword")

        if request.form.get("new_password") != request.form.get("confirm_password"):
            flash("Your new password needs to match the confirmation")
            return redirect("/changepassword")

        # ----------------------------------------------

        # Get the username
        user = getusername(session["user_id"])

        # Query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=user)

        # Ensure username exists and password is correct
        if not check_password_hash(rows[0]["hash"], request.form.get("current_password")):
            flash("Your current password doesn't match our records")
            return redirect("/changepassword")

        else:
            h = generate_password_hash(request.form.get("new_password"))
            db.execute("UPDATE users SET hash = :h WHERE username = :user", h=h, user=user)
            flash("Your password was successfully changed")
            return redirect("/")

    else:
        return render_template("password.html")

# -------------------------------------------


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# -------------------------------------------


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)


# -------------------------------------------