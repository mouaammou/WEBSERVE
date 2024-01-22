import cgi

def print(*args, **kwargs):
    kwargs["end"] = "\r\n"
    return __builtins__.print(*args, **kwargs)

cgi.print = print
cgi.test()
