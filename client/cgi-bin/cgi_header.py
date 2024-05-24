upload_not_found = '<html>\
                <head>\
                    <title>File Upload</title>\
                </head>\
                <body>\
                    <h1>File Upload Status</h1>\
                    <p>No file was uploaded</p>\
                </body>\
            </html>'

get_name_error = '<html>\
                <head>\
                    <title>Get Name</title>\
                </head>\
                <body>\
                    <h1>Error Get CGI</h1>\
                </body>\
            </html>'

http_not_found = f"HTTP/1.1 500 Internal Server Error\r\n\
                Content-Type: text/html\r\n\
                Content-Length: \r\n\
                \r\n"

http_found = f"HTTP/1.1 200 Ok\r\n\
                Content-Type: text/html\r\n\
                Content-Length: \r\n\
                \r\n"

html_head = f'<head> \
            <meta charset="UTF-8"> \
            <meta name="viewport" content="width=device-width, initial-scale=1.0"> \
            <title>Upload</title> \
            <link rel="stylesheet" href="../css/styles.css"> \
        </head>'

def get_header(host):
    header = f'<header> \
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
        </header>'
    return header