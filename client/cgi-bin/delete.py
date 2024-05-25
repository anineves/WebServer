#!/usr/bin/python3

from cgi_header import *
import os, cgi

form = cgi.FieldStorage()
host = os.environ.get("HTTP_HOST", default=None);
filename = form.getvalue('filename')
upload_dir = "./cgi-bin/uploads/"
if filename:
    to_remove = upload_dir + filename
    if os.path.isfile(to_remove):
        os.remove(to_remove)
        print(http_found)
        print('<html>')
        print(get_head("Delete"))
        print('<body>')
        print(get_header(host))
        print('<h1>File Removed</h1>')
        print(f'<p>File <strong>{filename}</strong> was removed successfully</p>')
        print('</body>')
        print('</html>')
    else:
        print(http_not_found)  
        print('<html>')
        print(get_head("Delete"))
        print('<body>')
        print(get_header(host))
        print(f'<h1>Oops... File "{filename}" was not found</h1>')
        print('<p>These are the files you can delete: </p>')
        files = os.listdir(upload_dir)
        print('<ul>')
        for file in files:
            print(f'<li>{file}</li>')
        print('</ul>')
        print('</body>')
        print('</html>')

else:
    print(http_not_found)
    print('<html>')
    print(get_head("Delete"))
    print('<body>')
    print(get_header(host))
    print(f'<h1>You need to write the file name!</h1>')
    print('<p>These are the files you can delete: </p>')
    files = os.listdir(upload_dir)
    print('<ul>')
    for file in files:
        print(f'<li>{file}</li>')
    print('</ul>')
    print('</body>')
    print('</html>')