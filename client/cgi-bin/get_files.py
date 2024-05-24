#!/usr/bin/python3

from cgi_header import *
import os, cgi

host = os.environ.get("HTTP_HOST", default=None);

arguments = cgi.FieldStorage()
return_error = False

#while True:
#    continue

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
    print(html_head)
    print(get_header(host))
    print(get_name_error)
else:
    response = f'<html>\
                    <head> \
                    <meta charset="UTF-8"> \
                    <meta name="viewport" content="width=device-width, initial-scale=1.0"> \
                    <title>Upload</title> \
                    <link rel="stylesheet" href="../css/styles.css"> \
                    </head>\
                <header> \
                    <div class="logo"> \
                    <img src="../assets/logo42_dark.svg" alt="Logo"> \
                    </div> \
                    <nav> \
                    <ul> \
                        <li><a href="http://{host}/index.html">Home</a></li> \
                        <li><a href="http://{host}/cgi.html">CGI</a></li> \
                    </ul> \
                    </nav> \
                </header>\
                    <body>\
                    <h1>Fuck You {name}!!!</h1>\
                    </body>\
                </html>'

    print(http_found)
    print(html_head)
    get_header(host)
    print(response)