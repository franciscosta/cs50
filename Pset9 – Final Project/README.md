


CS50x Final Project 2020
Organizer – Bookmarking, simplified.


1. Pitch:
Most of us are often trying to remember a couple good links.
Organizer is a minimal web based bookmarking tool.
Simply copy paste a link and hit enter.
Organizer pulls out all the relevant information and organizes it for you.


2. How it works:
The user copy pastes a url into the main input and hits enter.
Organizer scrapes the page and extracts the title, description and source url.
It then matches that information against an internal database where popular sites are catalogued.
Some of the categories in that catalogue include media, news, audio, shopping, ...
If the user bookmarks a url such as netflix.com (a url that matches that catalogue) the url is categorized as media and given the netflix's color.
If the url is by some reason not scrappable, the user is still redirected to a /create page where she can manually enter the url.
Over time, as she grows her list of bookmarks, she can filter her bookmarks by category or by url.
This can be done either via the filter page (listed on the menu) or by clicking on the source url on the entry itself.
All entries can also be edited in case the scrapped title or description are unclear.
Organizer also has some built-in logic to filter out the brand name for the title. 
Think Twitter.com where every page comes with '– Twitter' pre-baked into the headline.
The site is responsive and works across most devices and resolutions.


3. Sitemap:
/welcome – a landing page for non logged in users
/ signup – where the user can create an account (most exceptions are covered such as passwords not matching, email already registered on the db, ...)
/ login – where the user can log in.
/ – the feed of bookmarks, the main view in the application.
/filter – where the user can see all possible filters based on her bookmarks
/category/<category-name> – filter the feed to only show posts of that category
/url/<url-slug> – filter the feed to only show posts of that website
/create – a simple form where the user can manually enter a url
/edit/<bookmark-id> – where the user can edit or delete her bookmark
/changepassword – where the user can change her password
/logout – logout link, redirects the user back to /welcome


4. Technologies used:
In terms of Python, it uses Flask, SQLAlchemy, Werkzeug.Exceptions, Requests, Extraction, BeautifulSoup, and SQLite3.
In terms of frontend, it uses HTML and CSS and some minimal Javascript.
In terms of deployment, it uses Git, Gunicorn and Heroku.
Lastly, Organizer was developed in a virtual environment.


