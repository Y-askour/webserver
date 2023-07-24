#!/usr/bin/python3

from http import cookies
import datetime
import os
import cgi

form = cgi.FieldStorage()


# username = form.getvalue('username');
# password = form.getvalue('password');

array = os.environ['HTTP_COOKIE'].split(';')
# if not username or not password:
#     print('<p>cookie: is not set</p>')
# else:
values = {'usernmae': username, 'password': password}

expires = datetime.datetime.now() + datetime.timedelta(days = 365)
cookie = cookies.SimpleCookie(values)

for key in values:
    cookie[key] = values[key]
    cookie[key]['expires'] = expires.strftime("%a, %d-%b-%Y %H:%M:%S PST")
    cookie[key]['HttpOnly'] = True
    cookie[key]['path'] = '/'

for key in values:
    print('%s ' % cookie[key])

# if len(cookie) == 0:
#     print('<p>cookie: is not set</p>')
# else: