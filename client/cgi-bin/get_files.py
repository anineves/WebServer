#!/usr/bin/python3

import os, cgi

host = os.environ.get("HTTP_HOST", default=None);

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
                </ul> \
                </nav> \
            </header>\
                <body>\
                <h1>Fuck You!</h1>\
                </body>\
            </html>'

print(response)
