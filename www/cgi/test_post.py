#!/usr/bin/env python3
import cgi
import os

def print(*args, **kwargs):
    kwargs["end"] = "\r\n"
    return __builtins__.print(*args, **kwargs)
cgi.print = print

print("Content-Type: text/html")
print()

if os.environ['REQUEST_METHOD'] == 'POST':
    print("<html>")
    print("<head><title>POST Request</title></head>")
    print("<body>")
    print("<h1>POST Request Received</h1>")
    while True:
        try:
            line = input()
            print(line)
        except EOFError:
            break
    print("</body>")
    print("</html>")
else:
    print("<html>")
    print("<head><title>GET Request</title></head>")
    print("<body>")
    print("<h1>GET Request Received</h1>")
    print("<form method='post' action='test_post.py'>")
    # print("<p>Enter Body: <input type='textarea' name='body' /></p>")
    print("<textarea name=\"body\" cols=\"40\" rows=\"5\"></textarea>")
    print("<input type='submit' value='Submit' />")
    print("</form>")
    print("</body>")
    print("</html>")


