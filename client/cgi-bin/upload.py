#!/usr/bin/python3

import os, cgi

form = cgi.FieldStorage()
host = os.environ.get("HTTP_HOST", default=None);

not_found = '<html>\
                <head>\
                    <title>File Upload</title>\
                </head>\
                <body>\
                    <h1>File Upload Status</h1>\
                    <p>No file was uploaded</p>\
                </body>\
            </html>'

fileitem = form['file']
if fileitem.filename:
    upload_dir = "./cgi-bin/uploads/"
    with open(os.path.join(upload_dir, fileitem.filename), 'wb') as f:
        f.write(fileitem.file.read())
        print('<html>')
        print ('<head> \
                    <meta charset="UTF-8"> \
                    <meta name="viewport" content="width=device-width, initial-scale=1.0"> \
                    <title>Upload</title> \
                    <link rel="stylesheet" href="../css/styles.css"> \
                </head>')
        print('<body>')
        print(f'<header> \
                <div class="logo"> \
                <img src="../assets/logo42_dark.svg" alt="Logo"> \
                </div> \
                <nav> \
                <ul> \
                    <li><a href="http://{host}/index.html">Home</a></li> \
                    <li><a href="http://{host}/upload.html">Upload</a></li> \
                    <li><a href="http://{host}/delete.html">Delete</a></li> \
                </ul> \
                </nav> \
                </header>')
        print('<body>')
        print('<h1>File Upload Status</h1>')
        print(f'<p>File <strong>{fileitem.filename}</strong> was uploaded successfully</p>')
        print('<p>This are your files: </p>')
        files = os.listdir(upload_dir)
        print('<ul>')
        for file in files:
            print(f'<li>{file}</li>')
        print('</ul>')
        print('</body>')
        print('</html>')
else:
    http_header = f"HTTP/1.1 404 Not Found\r\n\
                    Content-Type: text/html\r\n\
                    Content-Length: << {len(not_found)} << \r\n\
                    \r\n"
    print(http_header)
    print(not_found)
