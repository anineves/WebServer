#!/usr/bin/python3

from cgi_header import *
import os, cgi

host = os.environ.get("HTTP_HOST", default=None);
arguments = cgi.FieldStorage()
for i in arguments.keys():
    name = arguments[i].value

while True:
    continue

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