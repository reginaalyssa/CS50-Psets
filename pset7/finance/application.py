from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session, url_for
from flask_session import Session
from passlib.apps import custom_app_context as pwd_context
from tempfile import mkdtemp

from helpers import *

# configure application
app = Flask(__name__)

# ensure responses aren't cached
if app.config["DEBUG"]:
    @app.after_request
    def after_request(response):
        response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
        response.headers["Expires"] = 0
        response.headers["Pragma"] = "no-cache"
        return response

# custom filter
app.jinja_env.filters["usd"] = usd

# configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.route("/")
@login_required
def index():
    # retrieve symbol, name, and shares of stock from db
    rows = db.execute("""SELECT symbol, name, sum(shares) AS shares FROM transactions 
        JOIN stocks on transactions.stock_id = stocks.id WHERE user_id = :id 
        GROUP BY symbol""", id=session["user_id"])

    grand_total = 0
    for row in rows:

        # retrieve current price of stock
        quote = lookup(row["symbol"])
        price = quote["price"]
        total = float(price * row["shares"])

        grand_total += total

        row["price"] = usd(price)
        row["total"] = usd(total)

    # get user's current cash balance
    cash = db.execute("SELECT * FROM users WHERE id = :id", id=session["user_id"])[0]["cash"]

    grand_total += cash

    # convert prices to USD format
    cash = usd(cash)
    grand_total = usd(grand_total)

    return render_template("index.html", stocks=rows, cash=cash, grand_total=grand_total)


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock."""

    # if user reached route via POST
    if request.method == "POST":

        # ensure symbol provided
        if not request.form.get("symbol"):
            return apology("missing symbol")

        # ensure symbol is valid and existing
        elif not lookup(request.form.get("symbol")):
            return apology("invalid symbol")

        # ensure shares provided
        elif not request.form.get("shares"):
            return apology("missing shares")

        # ensure shares is integer
        elif not request.form.get("shares").isnumeric():
            return apology("invalid shares")

        # ensure shares is positive integer
        elif int(request.form.get("shares")) <= 0:
            return apology("invalid shares")

        else:
            # retrieve stock quote from Yahoo! Finance
            quote = lookup(request.form.get("symbol"))

            # stock quote not retrieved successfully
            if not quote:
                return apology("invalid symbol")

            # get price of stock
            stock_price = quote["price"]

            # get amount of shares
            shares = int(request.form.get("shares"))

            # retrieve user's cash
            cash = db.execute("SELECT * FROM users WHERE id = :id",
                              id=session["user_id"])[0]["cash"]

            # compute for actual price of shares
            shares_price = stock_price * shares

            # user can afford buying
            if shares_price <= cash:

                # try to retrieve stock from database
                stock = db.execute("SELECT * FROM stocks WHERE symbol = :symbol",
                                   symbol=quote["symbol"])

                # if stock not yet in database, store it
                if not stock:
                    stock_id = db.execute("""INSERT INTO stocks(name, symbol) 
                        VALUES (:name, :symbol)""", name=quote["name"], symbol=quote["symbol"])
                else:
                    stock_id = stock[0]["id"]

                db.execute("""INSERT INTO transactions(price, shares, user_id, stock_id)
                    VALUES (:price, :shares, :user_id, :stock_id)""",
                           price=stock_price, shares=shares, user_id=session["user_id"], stock_id=stock_id)
                db.execute("""UPDATE users SET cash=:cash WHERE id=:id""",
                           cash=(cash - shares_price), id=session["user_id"])

                # redirect user to home page
                return redirect(url_for("index"))

            # user cannot afford to buy shares
            else:
                return apology("can't afford")

    # if user reached route via GET
    else:
        return render_template("buy.html")


@app.route("/history")
@login_required
def history():
    """Show history of transactions."""
    rows = db.execute("""SELECT symbol, shares, price, date FROM transactions
        JOIN stocks on transactions.stock_id = stocks.id
        WHERE user_id = :id""", id=session["user_id"])
    for row in rows:
        row["price"] = usd(row["price"])
    return render_template("history.html", rows=rows)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in."""

    # forget any user_id
    session.clear()

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password")

        # query database for username
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # ensure username exists and password is correct
        if len(rows) != 1 or not pwd_context.verify(request.form.get("password"), rows[0]["hash"]):
            return apology("invalid username and/or password")

        # remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # redirect user to home page
        return redirect(url_for("index"))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out."""

    # forget any user_id
    session.clear()

    # redirect user to login form
    return redirect(url_for("login"))


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""

    # if user reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # retrieve stock quote from Yahoo! Finance
        quote = lookup(request.form.get("symbol"))

        # stock quote not retrieved successfully
        if not quote:
            return apology("invalid symbol")

        # stock quote available
        else:
            return render_template("quoted.html", stock=quote["name"], symbol=quote["symbol"], price=usd(quote["price"]))

    # else if user reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user."""

    # if user reached route via POST
    if request.method == "POST":

        # ensure username was submitted
        if not request.form.get("username"):
            return apology("missing username")

        # ensure password was submitted
        elif not request.form.get("password"):
            return apology("missing password")

        # ensure passwords match
        elif request.form.get("password") != request.form.get("confirm_password"):
            return apology("passwords do not match")

        # add user to database if not yet existing
        else:
            # hash password before storing in database
            hash = pwd_context.hash(request.form.get("password"))
            result = db.execute("""INSERT INTO users (username, hash)
                VALUES (:username, :hash)""", username=request.form.get("username"), hash=hash)
            if not result:
                return apology("username taken")

            # log in newly registered user
            rows = db.execute("SELECT * FROM users WHERE username = :username",
                              username=request.form.get("username"))
            session["user_id"] = rows[0]["id"]

            return redirect(url_for("index"))

    # if user reached route via GET, display form
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock."""

    # if user reached route via POST
    if request.method == "POST":

        # ensure symbol provided
        if not request.form.get("symbol"):
            return apology("missing symbol")

        # ensure symbol is valid and existing
        elif not lookup(request.form.get("symbol")):
            return apology("invalid symbol")

        # ensure shares provided
        elif not request.form.get("shares"):
            return apology("missing shares")

        # ensure shares is integer
        elif not request.form.get("shares").isnumeric():
            return apology("invalid shares")

        # ensure shares is positive integer
        elif int(request.form.get("shares")) <= 0:
            return apology("invalid shares")

        else:
            # retrieve stock quote from Yahoo! Finance
            quote = lookup(request.form.get("symbol"))

            # stock quote not retrieved successfully
            if not quote:
                return apology("invalid symbol")

            # get amount of shares
            shares = int(request.form.get("shares"))

            # retrieve shares of stock user has bought
            user_shares = db.execute("""SELECT sum(shares) AS shares FROM transactions
                JOIN stocks on transactions.stock_id = stocks.id
                WHERE symbol = :symbol AND user_id = :id GROUP BY symbol""",
                                     symbol=request.form.get("symbol"), id=session["user_id"])

            # stock share hasn't been bought by user
            if len(user_shares) != 1:
                return apology("do not have shares")

            # shares can be sold since user has enought shares
            if user_shares[0]["shares"] >= shares:

                # get price of stock
                stock_price = quote["price"]

                # compute for actual price of shares
                shares_price = stock_price * shares

                # retrieve stock from database
                stock = db.execute("SELECT * FROM stocks WHERE symbol = :symbol",
                                   symbol=quote["symbol"])
                stock_id = stock[0]["id"]

                # update transactions table with sold shares
                db.execute("""INSERT INTO transactions(price, shares, user_id, stock_id)
                    VALUES (:price, :shares, :user_id, :stock_id)""", price=stock_price,
                           shares=-shares, user_id=session["user_id"], stock_id=stock_id)

                # retrieve and update user's cash balance
                cash = db.execute("SELECT * FROM users WHERE id = :id",
                                  id=session["user_id"])[0]["cash"]
                db.execute("UPDATE users SET cash=:cash WHERE id=:id",
                           cash=(cash + shares_price), id=session["user_id"])

                # redirect user to home page
                return redirect(url_for("index"))

            # user doesn't have enough shares to sell
            else:
                return apology("too many shares")

    # if user reached route via GET
    else:
        return render_template("sell.html")


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Changes user's password."""

    # if user reached route via POST
    if request.method == "POST":

        # ensure fields are not empty
        if not request.form.get("old_password"):
            return apology("missing old password")
        elif not request.form.get("new_password"):
            return apology("missing new password")

        # ensure old password is correct
        user = db.execute("SELECT * FROM users where id = :id", id=session["user_id"])[0]
        old_pw = user["hash"]
        if not pwd_context.verify(request.form.get("old_password"), old_pw):
            return apology("incorrect old password")

        # ensure passwords match
        elif request.form.get("new_password") != request.form.get("confirm_password"):
            return apology("new password doesn't match")

        # change password of user
        hash = pwd_context.hash(request.form.get("new_password"))
        db.execute("UPDATE users SET hash = :hash WHERE id = :id", hash=hash, id=session["user_id"])

        return redirect(url_for("index"))

    # if user reached route via GET
    else:
        user = db.execute("SELECT * FROM users where id = :id", id=session["user_id"])
        return render_template("password.html", user=user[0])
