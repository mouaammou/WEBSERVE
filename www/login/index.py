#!/usr/bin/python3
from http import cookies
import os
import cgi

# manage session cookies
def get_session():
    if 'HTTP_COOKIE' in os.environ:
         cookies = os.environ['HTTP_COOKIE']
         cookies = cookies.split('; ')
         for cookie in cookies:
              (_name, _value) = cookie.split('=')
              if 'session' == _name.lower():
                return _value
    return ''

# manage cookies
def get_cookie(match):
   if 'HTTP_COOKIE' in os.environ:
       cookies = os.environ['HTTP_COOKIE']
       cookies = cookies.split('; ')
       for cookie in cookies:
           (_name, _value) = cookie.split('=')
           if match.lower() == _name.lower():
               return _value
   return ''

def login_form():
    if get_cookie('UserID') != '' and get_session() != '':
        print('Location: welcome.html')
        print('Status: 302 Found')
        print('Content-Type: text/html; charset=utf-8')
        print()
        print('<html><body>Redirecting...</body></html>')
    else:
        print('Location: login.html')
        print('Status: 302 Found')
        print('Content-Type: text/html; charset=utf-8')
        print()
        print('<html><body>Redirecting...</body></html>')

def check_login(username, password):
   # Replace this with your own authentication logic
   if username and password:
       return True
   else:
       return False

def parse_post_data():
   form = cgi.FieldStorage()
   username = form.getvalue('username')
   password = form.getvalue('password')
   return username, password

if __name__ == "__main__":
    if 'REQUEST_METHOD' in os.environ and os.environ['REQUEST_METHOD'] == 'POST':
        username, password = parse_post_data()
        if username is not None and password is not None:
            if check_login(username, password):
                C = cookies.SimpleCookie()
                C["session"] = os.urandom(16).hex()
                C["UserID"] = username
                print(C)
                print('Location: welcome.html')
                print('Status: 302 Found')
                print('Content-Type: text/html; charset=utf-8')
                print()
                print('<html><body>Redirecting...</body></html>')
            else:
                print('Content-Type: text/html; charset=utf-8')
                print()
                print('<html><body><h1>Invalid login!</h1></body></html>')
        else:
            login_form()
    else:
        login_form()