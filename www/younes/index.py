import cgi 

form = cgi.FieldStorage()
if form != -1 : 
    username = form.getvalue('username')
    password = form.getvalue('password')

print("<html>")
print("<head>")
print("</head>")
print("<body>")
print("<h1>Python-CGI Login Page</h1>")
print("<p>Username: %s</p>" % username)
print("<p>Password: %s</p>" % password)
print("</body>")
print("</html>")
