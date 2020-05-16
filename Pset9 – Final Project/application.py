
import os, datetime
from flask import Flask, request, session, redirect, render_template, abort, jsonify, url_for
from flask_sqlalchemy import SQLAlchemy
from sqlalchemy import desc, DateTime
from sqlalchemy.exc import IntegrityError

from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash
from flask_mail import Mail, Message

import time

import requests, extraction
from bs4 import BeautifulSoup
import lxml
from urlextract import URLExtract
from urllib.parse import urlparse

from helpers import login_required

app = Flask(__name__)

app.config['SQLALCHEMY_DATABASE_URI'] = 'sqlite:///database.db'
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config['USER_ENABLE_EMAIL'] = True
app.secret_key = 'secretkeyisunique_1'

app.config['MAIL_SERVER'] = 'smtp.googlemail.com'
app.config['MAIL_PORT'] = 465
app.config['MAIL_USE_SSL'] = True
app.config['MAIL_USERNAME'] = 'yvesappemail@gmail.com'  
# app.config['MAIL_PASSWORD'] = os.getenv('MAIL_PASSWORD')

db = SQLAlchemy(app)
mail = Mail(app)

ext = extraction.Extractor()

# -------------------------------------------

# Objects

class User(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    username = db.Column(db.String(80), unique=True, nullable=False)
    email = db.Column(db.String(120), unique=True, nullable=False)
    passwordhash = db.Column(db.String(120), nullable=False)

# Categories of websites
class Category(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(200), nullable=False)

# Websites I have marked
class Link(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    linktype = db.Column(db.String(100), nullable=False)
    name = db.Column(db.String(300), nullable=False)
    url = db.Column(db.String(800), nullable=False)
    color = db.Column(db.String(10), nullable=False)
    categoryid = db.Column(db.Integer, db.ForeignKey('category.id'), nullable=False)

# Bookmarks themselves
class Bookmark(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    url = db.Column(db.String(800), nullable=False)
    name = db.Column(db.String(300), nullable=True)
    description = db.Column(db.String(2000), nullable=True)
    linktype = db.Column(db.String(100), nullable=False)
    sourceurl = db.Column(db.String(200), nullable=False)
    color = db.Column(db.String(10), nullable=True)
    date = db.Column(db.DateTime, server_default=db.func.now())
    userid = db.Column(db.Integer, db.ForeignKey('user.id'), nullable=False)
    userusername = db.Column(db.Integer, db.ForeignKey('user.username'), nullable=False)
    typeid = db.Column(db.Integer, db.ForeignKey('link.id'), nullable=False)
    categoryid = db.Column(db.Integer, db.ForeignKey('category.id'), nullable=False)



# -------------------------------------------

# Userful functions

def extract(url):
    extractor = URLExtract()
    find = extractor.find_urls(url)
    return find

def http(url):
    if 'http' not in url:
        url = 'http://' + url
    return url

# -------------------------------------------



'''
Structure:

0. Landing Page

1. Homepage
2. Edit
3. Create
3. Delete

4. Filter
5. Category pages
6. URL pages

7. Log In
8. Sign Up
9. Log Out
10. Change Password

11. 404


'''



# -------------------------------------------

# Landing

@app.route('/welcome')
def welcome():

    return render_template('welcome.html')


# -------------------------------------------

# Filter

@app.route('/filter')
def filter():

    linktype = db.engine.execute("SELECT DISTINCT linktype FROM bookmark WHERE userid = :userid", userid=session['user_id'])
    sourceurl = db.engine.execute("SELECT DISTINCT sourceurl, color FROM bookmark WHERE userid = :userid", userid=session['user_id'])
    # color = db.engine.execute("SELECT DISTINCT color FROM bookmark WHERE userid = :userid", userid=session['user_id'])



    return render_template('filter.html', linktype=linktype, sourceurl=sourceurl)


# -------------------------------------------

# Homepage

@app.route('/', methods=['POST', 'GET'])
@login_required
def index():
    

    if request.method == 'POST':

        # Get the url
        url = request.form.get('url')

        if not extract(url):
            return redirect('/') # Temporary

        # Make sure it's got an htt:// at the start
        url = http(url)

        r = requests.get(url)

        # Get the soup of info
        soup = ext.extract(r.text, source_url=url)

        # Handle errors
        if r.status_code != 200:
            # return redirect('/create') 
            error = "You need to add that one manually"
            return render_template('create.html', error=error)

        # Extract the single url
        sourceurl = urlparse(url).netloc
        sourceurl = sourceurl.replace('www.', '')

        # Remove unecessary things from the url
        n = soup.title.split("|", 1)
        finaltitle = n[0].split("(", 1)

        # Match it to the Link database
        match = Link.query.filter_by(url=sourceurl).first()

        # Get username
        u = User.query.filter_by(id=session['user_id']).first()

        if match:
            # Create the bookmark entry
            newBookmark = Bookmark( url=url,
                                    name=finaltitle[0],
                                    description=soup.description,
                                    linktype=match.linktype,
                                    sourceurl=match.url,
                                    color=match.color,
                                    userid=session['user_id'],
                                    userusername=u.username,
                                    typeid=match.id,
                                    categoryid=match.categoryid )

        elif not match:
            # Create the bookmark entry
            newBookmark = Bookmark( url=url,
                                    name=finaltitle[0],
                                    description=soup.description,
                                    linktype='bookmark',
                                    sourceurl=sourceurl,
                                    color='#000000',
                                    userid=session['user_id'],
                                    userusername=u.username,
                                    typeid='0',
                                    categoryid='1' )

        try:
            db.session.add(newBookmark)
            db.session.commit()
        except IntegrityError:
            return redirect('/')

        # return render_template('index.html', o=finalObject)
        return redirect('/')

    else:

        # For the pagination
        page = request.args.get('page', 1, type=int)

        # Get all my bookmarks
        listBookmarks = Bookmark.query.filter_by(userid=session['user_id']).order_by(desc(Bookmark.date)).paginate(page=page)

        return render_template('index.html', post=listBookmarks)


# -------------------------------------------

# Edit

@app.route('/edit/<int:bookmark_id>', methods=['POST', 'GET'])
@login_required
def edit(bookmark_id):

    bookmark = Bookmark.query.filter_by(id=bookmark_id).first()

    user = User.query.filter_by(id=session['user_id']).first()

    # bookmark = Bookmark.query.get_or_404(bookmark_id).first()

    if request.method == 'POST':

        # Get the data from the form
        name = request.form.get('name')
        description = request.form.get('description')

        # Make sure everything is filled
        if not name or not description:
            return redirect('/edit/' + str(bookmark_id) + '') # Temporary

        bookmark.name = name
        bookmark.description = description
        db.session.commit()

        return redirect('/')

    else:
        if bookmark.userid != session['user_id']:
            abort(403)
        return render_template('edit.html', post=bookmark)


# -------------------------------------------


@app.route('/create', methods=['POST', 'GET'])
@login_required
def create():

    if request.method == 'POST':

        url = request.form.get('url')
        name = request.form.get('name')
        description = request.form.get('description')

        # Make sure the url is a url
        if not extract(url):
            return redirect('/') # Temporary

        # Make sure it's got an http:// at the start
        url = http(url)

        # Extract the single url
        sourceurl = urlparse(url).netloc
        sourceurl = sourceurl.replace('www.', '')

        # Match it to the Link database
        match = Link.query.filter_by(url=sourceurl).first()

        # Get username
        u = User.query.filter_by(id=session['user_id']).first()

        if match:
            # Create the bookmark entry
            newBookmark = Bookmark( url=url,
                                    name=name,
                                    description=description,
                                    linktype=match.linktype,
                                    sourceurl=match.url,
                                    color=match.color,
                                    userid=session['user_id'],
                                    userusername=u.username,
                                    typeid=match.id,
                                    categoryid=match.categoryid )

        elif not match:
            # Create the bookmark entry
            newBookmark = Bookmark( url=url,
                                    name=name,
                                    description=description,
                                    linktype='bookmark',
                                    sourceurl=sourceurl,
                                    color='#000000',
                                    userid=session['user_id'],
                                    userusername=u.username,
                                    typeid='0',
                                    categoryid='1' )

        try:
            db.session.add(newBookmark)
            db.session.commit()
        except IntegrityError:
            return redirect('/')


        return redirect('/')

    else:

        return render_template('create.html')



# -------------------------------------------


@app.route('/delete/<int:bookmark_id>')
@login_required
def delete(bookmark_id):

    bookmark = Bookmark.query.filter_by(id=bookmark_id).first()

    if bookmark:
        db.session.delete(bookmark)
        db.session.commit()
        return redirect('/')

    if not bookmark:
        return redirect('/')



# -------------------------------------------


@app.route('/category/<linktype>')
@login_required
def category(linktype):

    # For the pagination
    page = request.args.get('page', 1, type=int)

    u = session['user_id']

    # Get all my bookmarks
    listBookmarks = Bookmark.query.filter_by(linktype=linktype).filter_by(userid=u).order_by(desc(Bookmark.date)).paginate(page=page)

    return render_template('index.html', post=listBookmarks)



# -------------------------------------------


@app.route('/url/<url>')
@login_required
def url(url):

    # For the pagination
    page = request.args.get('page', 1, type=int)

    u = session['user_id']

    # Get all my bookmarks
    listBookmarks = Bookmark.query.filter_by(sourceurl=url).filter_by(userid=u).order_by(desc(Bookmark.date)).paginate(page=page)

    return render_template('index.html', post=listBookmarks)




# -------------------------------------------
# -------------------------------------------
# -------------------------------------------




# Login, Signup / Logout / Change Password

@app.route('/login', methods=['POST', 'GET'])
def login():

    session.clear()

    if request.method == 'POST':

        error = None

        # Check if fields are there
        if not request.form.get('username'):
            error = "Please enter your username"
            return render_template('login.html', error=error)

        if not request.form.get('password'):
            error = "Please enter your password"
            return render_template('login.html', error=error)

        # Check if username exists
        u = request.form.get('username')
        user = User.query.filter_by(username=u).first()

        # If user doesn't exist
        if not user:
            error = "That username doesn't match our records"
            return render_template('login.html', error=error)

        # If password
        if check_password_hash(user.passwordhash, request.form.get('password')) == False:
            error = "Oops, wrong password"
            return render_template('login.html', error=error)

        if error is None and check_password_hash(user.passwordhash, request.form.get('password')) == True:
            session['user_id'] = user.id
            return redirect('/')

    else:

        return render_template('login.html')



# -------------------------------------------


@app.route('/signup', methods=['POST', 'GET'])
def signup():

    if request.method == 'POST':

        error = None

        if not request.form.get('email'):
            error = "Please enter your email"
            return render_template('signup.html', error=error)

        if not request.form.get('username'):
            error = "Please enter your username"
            return render_template('signup.html', error=error)

        if not request.form.get('password'):
            error = "You forgot your password"
            return render_template('signup.html', error=error)

        if not request.form.get('confirmpassword'):
            error = "You need to confirm password"
            return render_template('signup.html', error=error)

        if request.form.get('password') != request.form.get('confirmpassword'):
            error = "Passwords need to match"
            return render_template('signup.html', error=error)

        u = request.form.get('username')
        e = request.form.get('email')
        
        uc = User.query.filter_by(username=u).first()
        ec = User.query.filter_by(email=e).first()

        if uc:
            error = "That username is taken"
            return render_template('signup.html', error=error)

        if ec:
            error = "There's already an account associated with that email"
            return render_template('signup.html', error=error)

        if error is None:

            p = generate_password_hash(request.form.get('password'))

            # Create the user object
            newuser = User(username=u, email=e, passwordhash=p)

            # Add to DB
            db.session.add(newuser)
            db.session.commit()

            user = User.query.filter_by(username=u).first()

            newBookmark = Bookmark( url='http://nationalgeographic.com',
                                    name='National Geographic: Stories of Animals, Nature, and Culture',
                                    description='Explore National Geographic. A world leader in geography, cartography and exploration.',
                                    linktype='news',
                                    sourceurl='nationalgeographic.com',
                                    color='#ffd23c',
                                    userid=user.id,
                                    userusername=user.username,
                                    typeid=43,
                                    categoryid=2 )

            db.session.add(newBookmark)
            db.session.commit()

            newBookmark = Bookmark( url='https://www.theatlantic.com/',
                                    name='The Atlantic',
                                    description='The Atlantic covers news, politics, culture, technology, health, and more, through its articles, podcasts, videos, and flagship magazine.',
                                    linktype='news',
                                    sourceurl='theatlantic.com',
                                    color='#e7121b',
                                    userid=user.id,
                                    userusername=user.username,
                                    typeid=42,
                                    categoryid=2 )

            db.session.add(newBookmark)
            db.session.commit()

            newBookmark = Bookmark( url='https://quibi.com',
                                    name='Quibi - Quick bites. Big stories.',
                                    description='Quick bites of captivating entertainment, created for mobile by the best talent, designed to fit perfectly into any moment of your day.',
                                    linktype='news',
                                    sourceurl='quibi.com',
                                    color='#a02cf6',
                                    userid=user.id,
                                    userusername=user.username,
                                    typeid=61,
                                    categoryid=2 )

            db.session.add(newBookmark)
            db.session.commit()

            # welcomemail(request.form.get('email'))

            return redirect('/login')

    else:

        return render_template('signup.html')




# -------------------------------------------


@app.route('/logout')
def logout():

    # Clear session
    session.clear()

    # Set message
    error = "Logged Out"

    # Send user back to login
    return redirect('/welcome')



# -------------------------------------------


@app.route('/changepassword', methods=['POST', 'GET'])
@login_required
def changepassword():

    if request.method == 'POST':

        error = None

        # Check that all fields have data

        if not request.form.get('currentpassword'):
            error = "You must first enter your current password"
            return render_template('changepassword.html', error=error)

        if not request.form.get('newpassword'):
            error = "You must enter a new password"
            return render_template('changepassword.html', error=error)

        if not request.form.get('confirmpassword'):
            error = "You must confirm your new password"
            return render_template('changepassword.html', error=error)

        if request.form.get('newpassword') != request.form.get('confirmpassword'):
            error = "Your new password must match the confirmation"
            return render_template('changepassword.html', error=error)

        # Get current user info

        user = User.query.filter_by(id=session['user_id']).first()

        # Check that current password is correct

        if check_password_hash(user.passwordhash, request.form.get('currentpassword')) == False:
            error = "Your current password might be wrong"
            return render_template('changepassword.html', error=error)

        # Update passwords

        if error is None and request.form.get('newpassword') == request.form.get('confirmpassword'):
            user.passwordhash = generate_password_hash(request.form.get('newpassword'))
            db.session.commit()

            error = "Password changed"
            return render_template('changepassword.html', error=error)


    else:

        return render_template('changepassword.html')




# -------------------------------------------

# Admin Page


@app.route('/admin', methods=['POST', 'GET'])
@login_required
def admin():

    categoryList = Category.query.all()

    linkList = Link.query.all()

    users = User.query.count()

    if request.method == 'POST':

        categ = request.form.get('categ')

        name = request.form.get('name')
        url = request.form.get('url')
        color = request.form.get('color')
        linktype = request.form.get('category')

        if name and url and color and linktype:

            category = Category.query.filter_by(name=linktype).first()

            newLink = Link(linktype=linktype, name=name, url=url, color=color, categoryid=category.id)

            try:
                db.session.add(newLink)
                db.session.commit()
            except IntegrityError:
                return redirect('/admin')

        if categ:

            newCategory = Category(name=categ)

            try:
                db.session.add(newCategory)
                db.session.commit()
            except IntegrityError:
                return redirect('/admin')

        return redirect('/admin')   

    else:

        return render_template('admin.html', categoryList=categoryList, linkList=linkList, users=users)




# -------------------------------------------

# 404

@app.errorhandler(404)
def page_not_found(e):
    error=e
    return redirect(url_for('index', message=error))