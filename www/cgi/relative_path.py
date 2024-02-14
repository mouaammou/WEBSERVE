import cgi
import os
import sys
print("Content-Type: text/html", end="\r\n")
print(end="\r\n")
print("<html><head><title>Path</title></head><body>")
print("<h1>PATH</h1>")
print(f"<p>path of current cgi script: {sys.argv[0]}</p>")
print(f"<p>current working directory : {os.getcwd()}</p>")
print("</body></html>")

