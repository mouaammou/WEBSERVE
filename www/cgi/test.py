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
print('Content-sdsType: text/plain; charset=utf-8')
print('Content-sdsType: text/plain; charset=utf-8')
print()
# print(os.environ)
for i in os.environ:
    print(i,"--->", os.environ[i])

print("\n\n----------------------------------------------\n")
while True:
    try:
        print(input())
    except:
        break