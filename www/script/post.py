#!/usr/local/bin/python3

# import cgi
# import os
# import sys

# upload_dir = os.environ['UPLOAD_DIR']
# print("hada script")
# print(sys.stdin.buffer.read())
# #print(upload_dir)

# form = cgi.FieldStorage()
# if len(form) == 0:
#     print("error in cgi")

# if 'file' in form:
#     print("ana ghadi ldar")
#     # Get the uploaded file
#     file_item = form['file']
#     upload_dir = os.environ['UPLOAD_DIR']

#     # Check if the file was uploaded
#     if file_item.filename:
#         # Open a file for writing
#         file = open(upload_dir + file_item.filename, 'wb')

#         # Write the file to the server
#         file.write(file_item.file.read())
#         file.close()
#         message = 'File Uploaded Successfully'
#     else:
#         message = "Error uploading file";
# else:
#     message = 'No file was uploaded'

# #print("Content-type: text/html\n")
# print("<html>")
# print("<body>")
# print("<p>%s</p>" % message)
# print("</body>")
# print("</html>")



####here the code
import cgi
import os
import sys

form = cgi.FieldStorage()
if len(form) == 0:
    print("error in cgi")

#print(sys.stdin.buffer.read())
#print("hadi 7kaya")

if 'file' in form:
    # Get the uploaded file
    file_item = form['file']
    upload_dir = os.environ['UPLOAD_DIR']

    # Check if the file was uploaded
    if file_item.filename:
        # Open a file for writing
        print(file_item.filename)
        print("ok")
        print(upload_dir + file_item.filename)
        file = open(upload_dir + file_item.filename, 'wb')

        # Write the file to the server
        file.write(file_item.file.read())
        file.close()
        message = 'File Uploaded Successfully'
    else:
        message = "Error uploading file";
else:
    message = 'No file was uploaded'

#print("Content-type: text/html\n")
print("<html>")
print("<body>")
#print("klsjkljsdkl")
print("<p>%s</p>" % message)
print("</body>")
print("</html>")
