#!/usr/local/bin/python3

import sys
import cgi
import os


form = cgi.FieldStorage()

if 'file' in form:
    # Get the uploaded file
    file_item = form['file']
    upload_dir = os.environ['UPLOAD_DIR']

    # Check if the file was uploaded
    if file_item.filename:
        # Open a file for writing
        file = open(upload_dir + file_item.filename, 'wb')

        # Write the file to the serverç
        file.write(file_item.file.read())
        file.close()
        message = 'File Uploaded Successfully'
    else:
        message = "Error uploading file";
else:
    message = 'No file was uploaded'

print("HTTP/1.1 200 OK\r")
print("Content-type: text/html\r\n\r")
print("<html>")
print("<head>")
print("    <title>Upload File</title>")
print("    <style>")
print("        body {")
print("            font-family: Arial, sans-serif;")
print("            background-color: #f2f2f2;")
print("            margin: 0;")
print("            padding: 20px;")
print("        }")
print("")
print("        h1 {")
print("            color: #333;")
print("            font-size: 32px;")
print("            margin-top: 0;")
print("            text-align: center;")
print("        }")
print("")
print("        p {")
print("            color: #666;")
print("            font-size: 18px;")
print("            margin-top: 20px;")
print("            text-align: center;")
print("        }")
print("    </style>")
print("</head>")
print("<body>")
print("<h1>File Upload</h1>")
print("<p>%s</p>" % message)
print("</body>")
print("</html>")