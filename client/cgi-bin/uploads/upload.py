#!/usr/bin/python3

from cgi_header import *
import os, cgi

form = cgi.FieldStorage()
host = os.environ.get("HTTP_HOST", default=None);

fileitem = form['file']
if fileitem.filename:
    upload_dir = "./cgi-bin/uploads/"
    with open(os.path.join(upload_dir, fileitem.filename), 'wb') as f:
        f.write(fileitem.file.read())
        print(http_found_upload)
        print('<html>')
        print(get_head("Upload"))
        print('<body>')
        print(get_header(host))        
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
    print(http_not_found)
    print(get_head("Upload"))
    print(get_header(host))        
    print(upload_not_found)
