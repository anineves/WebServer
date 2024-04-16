@@ -0,0 +1,21 @@
#!/usr/bin/python3

import cgi, os

form = cgi.FieldStorage()

# Get filename here
fileitem = form['file']

# Test if the file was uploaded
if fileitem.filename:
   open(os.getcwd() + '/cgi-bin/' + os.path.basename(fileitem.filename), 'wb').write(fileitem.file.read())
   message = 'The file "' + os.path.basename(fileitem.filename) + '" was uploaded to ' + os.getcwd() + '/cgi-bin/tmp'
   print ("OKOKOKOKOKOKOKOKOKOKOKOKOKOKOKOK")
else:
   message = 'Uploading Failed'
   print ("FAILEDFAILEDFAILEDFAILEDFAILED")

print("Content-Type: text/html;charset=utf-8")
print ("Content-type:text/html\r\n")
print("<H1> " + message + " </H1>")