#!/usr/bin/python3

from cgi_header import *
import os, cgi

host = os.environ.get("HTTP_HOST", default=None);


#while True:
#continue; 

arguments = cgi.FieldStorage()
return_error = False

for key in arguments:
    if key != "name":
        return_error = True

name = arguments.getvalue("name")
if (name is None):
    return_error = True

if isinstance(name, list):
    if (len(name) > 1):
        return_error = True

if return_error == True:
    print(http_not_found)
    print(get_head("Hello CGI"))
    print(get_header(host))
    print(get_name_error)
else:
    print(http_found)
    print("<html>")
    print(get_head("Hello CGI"))
    print("<body>")
    print(get_header(host))
    print(f"<h1>Hello by CGI {name}!!!</h1>")
    print("</body>")
    print("</html>")          