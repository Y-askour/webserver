#!/usr/bin/python3

from http import cookies
import datetime
import os
import cgi

form = cgi.FieldStorage()
values = {'usernmae': form.getvalue('username'), 'password': form.getvalue('password')}

if not values:
    print("empty")

days = 365
expires = datetime.datetime.now() + datetime.timedelta(days)
# cookie = cookies.SimpleCookie(os.environ.get('HTTP_COOKIE'))
cookie = cookies.SimpleCookie(values)

for key in values:
    cookie[key] = values[key]
    cookie[key]['expires'] = expires.strftime("%a, %d-%b-%Y %H:%M:%S PST")
    cookie[key]['path'] = '/'
    cookie[key]['HttpOnly'] = True


# header
print("Content-type:text/html\r\n\r\n")
# body
print("<html>")
print("<head>")
print("<title> cookies </title>")
print("</head>")
print("<body>")


if len(cookie) == 0:
    print('<p>cookie: is not set</p>')
else:
    for key in values:
        print('<p>Cookie: %s = %s</p>' % (key, values[key]))

print("</body>")
print("</html>")