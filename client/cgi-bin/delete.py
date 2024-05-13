#!/usr/bin/python3

import os, cgi

form = cgi.FieldStorage()

host = os.environ.get("HTTP_HOST", default=None);
filename = form.getvalue('filename')
upload_dir = "./cgi-bin/uploads/"
if filename:
    to_remove = upload_dir + filename
    if os.path.isfile(to_remove):
        os.remove(to_remove)
        print('<html>')
        print ('<head> \
                    <meta charset="UTF-8"> \
                    <meta name="viewport" content="width=device-width, initial-scale=1.0"> \
                    <title>Delete</title> \
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
                    <li><a href="http://{host}/delete.html">Delete</a></li> \
                    <li><a href="http://{host}/upload.html">Upload</a></li> \
                </ul> \
                </nav> \
                </header>')
        print('<h1>File Removed</h1>')
        print(f'<p>File <strong>{filename}</strong> was removed successfully</p>')
        print('</body>')
        print('</html>')
    else:
        print('<html>')
        print('<head>')
        print('</head>')
        print('<body>')
        print('<h1>File was not found</h1>')
        print('<p>This are your files: </p>')
        files = os.listdir(upload_dir)
        for file in files:
            print(f'<p>->{file}</p>')
        print('</body>')
        print('</html>')

else:
    print('<html>')
    print('<head>')
    print('</head>')
    print('<body>')
    print('<h1>File was not found</h1>')
    print('<p>This are your files: </p>')
    files = os.listdir(upload_dir)
    for file in files:
        print(f'<p>->{file}</p>')
    print('</body>')
    print('</html>')