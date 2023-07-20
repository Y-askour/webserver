import cgi 

form = cgi.FieldStorage()

username = form.getvalue('username')
password = form.getvalue('password')

print("Content-type:text/html\r\n\r\n")
print("<html>")
print("<head>")
print("<title>Python-CGI Login Page</title>")
print("</head>")
print("<body>")
print("<h1>Python-CGI Login Page</h1>")
print("<p>Username: %s</p>" % username)
print("<p>Password: %s</p>" % password)
print("</body>")
print("</html>")
