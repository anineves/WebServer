#!/usr/bin/python3

import os, cgi

# form = cgi.FieldStorage()

# host = os.environ.get("HTTP_HOST", default=None);
# filename = form.getvalue('filename')

print('<html>')
    print ('<head> \
            <meta charset="UTF-8"> \
            <meta name="viewport" content="width=device-width, initial-scale=1.0"> \
            <title>Files</title> \
            <link rel="stylesheet" href="../css/styles.css"> \
            </head>')
    print('<body>')
    print(f'<header> \
                <div class="logo"> \
                <img src="../assets/logo42_dark.svg" alt="Logo"> \
                </div> \
                <nav> \
                <ul> \
                    <li><a href="#">Home</a></li> \
                    <li><a href="#">Upload</a></li> \
                    <li><a href="#">Delete</a></li> \
                </ul> \
                </nav> \
                </header>')
    print('<body>')
        print('<h1>FILES</h1>')
    print('</body>')
print('</html>')