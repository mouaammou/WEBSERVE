# from http import cookies

# # Create a new SimpleCookie object
# C = cookies.SimpleCookie()

# # Set some cookies
# C["UserID"] = "rthdrhtetrhert"
# C["Password"] = "XYZ123"

# # Print the cookies
# print(C)
# print()
import cgitb
cgitb.enable()
import os
print('Location: index.html')
print('Status: 302 Found')
print('Content-Type: text/html; charset=utf-8')
print()
print('<html><body>Redirecting...</body></html>')
