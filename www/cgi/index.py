#!/usr/bin/python3
from http import cookies
import os
import cgi
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
   print('Content-Type: text/html; charset=utf-8')
   print()
   print('<html><body>')
   print('<form method="POST" action="/cgi/test.py">')
   print('Username: <input type="text" name="username"><br>')
   print('Password: <input type="password" name="password"><br>')
   print('<input type="submit" value="Login">')
   print('</form>')
   print('</body></html>')

def check_login(username, password):
   # Replace this with your own authentication logic
   if username == 'admin' and password == 'secret':
       return True
   else:
       return False


if __name__ == "__main__":
   if 'REQUEST_METHOD' in os.environ and os.environ['REQUEST_METHOD'] == 'POST':
       if 'username' in os.environ and 'password' in os.environ:
           username = os.environ['username']
           password = os.environ['password']
           if check_login(username, password):
               C = cookies.SimpleCookie()
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
               print('<html><body>Invalid login!</body></html>')
       else:
           login_form()
   else:
       login_form()

# cgi.test()
# print("Content-Type: text/html")
# print()
# print('''<html>
# 		<head>
# 			<style>
# 				/* Add some basic styling */
# 				body {
# 					font-family: Arial, sans-serif;
# 					background-color: #f2f2f2;
# 					padding: 20px;
# 				}

# 				.form-container {
# 					max-width: 300px;
# 					margin: 0 auto;
# 					background-color: #fff;
# 					padding: 20px;
# 					border-radius: 5px;
# 					box-shadow: 0 2px 5px rgba(0, 0, 0, 0.1);
# 				}

# 				.form-container h2 {
# 					text-align: center;
# 					margin-bottom: 20px;
# 				}

# 				.form-container input[type="text"] {
# 					width: 100%;
# 					padding: 10px;
# 					margin-bottom: 10px;
# 					border-radius: 5px;
# 					border: 1px solid #ccc;
# 				}

# 				.form-container button[type="submit"] {
# 					width: 100%;
# 					padding: 10px;
# 					background-color: #4CAF50;
# 					color: #fff;
# 					border: none;
# 					border-radius: 5px;
# 					cursor: pointer;
# 				}

# 				.form-container button[type="submit"]:hover {
# 					background-color: #45a049;
# 				}
# 			</style>
# 		</head>
# 		<body>
# 			<div class="form-container">
# 				<h2>Submit User</h2>
# 				<form method="POST" action="test.py">
# 					<input type="text" name="user" placeholder="Enter username">
# 					<button type="submit">Submit</button>
# 				</form>
# 			</div>
# 		</body>
# 	</html>
# 	''')
# import os
# import Cookie

# # Function to parse cookies from the HTTP request
# def parse_cookies():
#     if 'HTTP_COOKIE' in os.environ:
#         cookie_str = os.environ['HTTP_COOKIE']
#         return Cookie.SimpleCookie(cookie_str)
#     else:
#         return None

# # Function to set a cookie
# def set_cookie(cookie_name, cookie_value, max_age=None):
#     cookie = Cookie.SimpleCookie()
#     cookie[cookie_name] = cookie_value

#     # Set additional cookie attributes as needed
#     if max_age is not None:
#         cookie[cookie_name]['max-age'] = max_age

#     # Print the Set-Cookie header
#     print(cookie.output())

# # Parse existing cookies
# cookies = parse_cookies()

# # Set a new cookie
# set_cookie('my_cookie', 'cookie_value', max_age=3600)  # Example: Cookie expires in 1 hour

# # Print other CGI headers
# print("Content-type: text/html\n")

# # Your CGI script content goes here
# print("<html><head><title>CGI with Cookie</title></head><body>")
# print("<h1>CGI Script with Cookie</h1>")
# print("<p>Cookie has been set. Check your browser's developer tools to see the cookie.</p>")
# print("</body></html>")
