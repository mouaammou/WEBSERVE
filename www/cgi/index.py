#!/usr/bin/env python

print("Content-type: text/html\n")  # Required header indicating the content type (HTML in this case)

# Your HTML content goes here
html_content = """
<!DOCTYPE html>
<html>
<head>
    <title>CGI Python Script</title>
</head>
<body>
    <h1>Hello, CGI World!</h1>
    <p>This is a simple CGI script written in Python.</p>
</body>
</html>
"""

# Print the HTML content to the standard output
print(html_content)
