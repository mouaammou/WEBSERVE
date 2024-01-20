import cgi
import cgitb

def print(*args, **kwargs):
    return __builtins__.print(*args, **kwargs,end='\r\n', flush=True)

# print("Cache-Control: test")
# print("Date: test")
# print("Server: test")
print()
print("<html><head><title>Test CGI</title></head><body>")
print("<h1 align=center>Test CGI</h1>")
print("<hr/>")
print("</body>")
print("</html>")
