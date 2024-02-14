import cgi

def print(*args, **kwargs):
    kwargs["end"] = "\r\n"
    return __builtins__.print(*args, **kwargs)

cgi.print = print
cgi.test()
# print("Content-Type: text/html")
# print()
# print("<html><body>")
# print("<h1>Hello World</h1>")
# print("</body></html>")


