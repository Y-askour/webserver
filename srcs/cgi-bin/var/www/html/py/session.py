#!/usr/bin/env python3

import datetime
import os
import string
import random
from http import cookies
days = 365
expires = datetime.datetime.now() + datetime.timedelta(days)

cookie = cookies.SimpleCookie()
client_cookie = cookies.SimpleCookie()

string_cookie = os.environ.get('HTTP_COOKIE')

if string_cookie:
    client_cookie.load(string_cookie)


def generat_new_session_id(length):
    letters = string.ascii_lowercase
    numbers = string.digits
    return ''.join(random.choice(letters + numbers) for _ in range(length))


if not client_cookie.get('session_id'):
    session_id = generat_new_session_id(26)

    cookie['session_id'] = session_id
    cookie['session_id']['expires'] = expires.strftime(
        "%a, %d-%b-%Y %H:%M:%S PST")
    cookie['session_id']['path'] = '/'
    cookie['session_id']['HttpOnly'] = True

print(cookie)
print('Content-type: text/html\r\n')
print('\r\n')
print('<html><body>')

if client_cookie.get('session_id'):
    print('<p>Welcome back!</p>')
else:
    print('<p>First time here?</p>')

print('</body></html>')
