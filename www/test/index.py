#!/usr/bin/env python3
from http import cookies
import os
import cgi
import cgitb
cgitb.enable()
print('Content-Type: text/html; charset=utf-8', end='\r\n')
print(end='\r\n')
print('<html><body><h1>Invalid login!</h1></body></html>')
exit(0)