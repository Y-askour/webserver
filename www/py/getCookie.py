#!/usr/bin/python

import cgi, cgitb 

form = cgi.FieldStorage() 

FirstName  = form.getvalue("FirstName")
LastName = form.getvalue("LastName")

print (LastName)
print (FirstName)