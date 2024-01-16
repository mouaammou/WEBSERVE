#!/usr/bin/env python

import cgi
import cgitb
import os
import json
from datetime import datetime, timedelta

# Enable CGI traceback for debugging
cgitb.enable()

def load_sessions():
    # Load existing session data from a file
    try:
        with open('sessions.json', 'r') as file:
            return json.load(file)
    except FileNotFoundError:
        return {}

def save_sessions(sessions):
    # Save session data to a file
    with open('sessions.json', 'w') as file:
        json.dump(sessions, file)

def generate_session_id():
    # Generate a unique session ID
    return os.urandom(16).hex()

def get_session():
    # Get the session ID from the user's cookie
    cookie = os.environ.get('HTTP_COOKIE')
    if cookie and 'session_id' in cookie:
        return sessions.get(cookie['session_id'])
    return None

def create_session():
    # Create a new session
    session_id = generate_session_id()
    session = {'session_id': session_id, 'username': None, 'expiry': str(datetime.utcnow() + timedelta(minutes=30))}
    sessions[session_id] = session
    save_sessions(sessions)
    return session

def set_session_cookie(session):
    # Set the session ID as a cookie in the response
    print(f"Set-Cookie: session_id={session['session_id']}; Path=/;")

# Load existing sessions
sessions = load_sessions()

# Get the current session
current_session = get_session()

# Check if the user is logged in
if current_session and current_session['username']:
    print(f"Content-Type: text/html\n")
    print(f"Logged in as {current_session['username']}. <a href='/logout'>Logout</a>")
else:
    # Display a login form
    form = cgi.FieldStorage()
    username = form.getvalue('username')

    if username:
        # Log in the user and create a new session
        current_session = create_session()
        current_session['username'] = username
        set_session_cookie(current_session)
        print(f"Content-Type: text/html\n")
        print(f"Logged in as {current_session['username']}. <a href='/logout'>Logout</a>")
    else:
        # Display the login form
        print(f"Content-Type: text/html\n")
        print("Content-Type: text/html\n")
        print("<form method='post' action='/'>")
        print("  Username: <input type='text' name='username'>")
        print("  <input type='submit' value='Login'>")
        print("</form>")

# Save session data after processing the request
save_sessions(sessions)
