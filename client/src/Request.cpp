#include "../includes/Request.hpp"

Request::Request(std::string request) : _fullRequest(request)
{

    has_header = false;
    _code = 200;
}

Request::Request() {
        has_header = false;
}

Request::~Request() {}

void Request::parser(std::string header)
{
    std::istringstream iss(header);
    std::stringstream ss(header);
    std::string line;

    iss >> _method;
    iss >> _path;
    iss >> _protocol;

    if (_path.find('?') != std::string::npos)
    {
        _query = _path.substr(_path.find('?') + 1, _path.length());
        _path = _path.substr(0, _path.find('?'));
    }

    while (std::getline(ss, line) && line != "\r")
    {
        // std::cout << "linha:::::" << line << std::endl;
        if (line.find(':') != std::string::npos)
        {
            std::string name(line.substr(0, line.find(':')));
            std::string content(line.substr(line.find(':') + 2, line.find('\n')));
            if (content.length() != 0)
            {

                this->lines_header[name] = content;
            }
        }
        std::stringstream ss(line);
        std::string fword;
        std::string sword;
        ss >> fword;
        ss >> sword;
        if (fword == "Content-Length:")
        {
            _contentLength = sword;
        }
        if (fword == "Content-Type:")
        {
            std::string content_word;
            ss >> content_word;
            sword += " " + content_word;
            _contentType = sword;
        }
        if (fword == "Host:")
        {
            _host = sword;
        }
    }
    this->has_header = true; 

    while (std::getline(ss, line) && line != "\r")
    {
        //std::cout << "linha body:::::" << line << std::endl;
        //if (line.find('=') != std::string::npos)
        //{
          //  std::string name(line.substr(0, line.find('=')));
            //std::string content(line.substr(line.find('=') + 1, line.find('\n')));
            //if (content.length() != 0)
            //{

                this->lines_body += line;
            //}
        //}
    }

    // std::cout << MAGENTA << "LINHAS " << this->lines_body << RESET << std::endl;

   
}

bool Request::verific_errors(Server server)
{
    // Aqui Depois em vez do exitWithError colocar os erros, por exemplo se o metodo for diferente do esperado e o erro 501
    if (_method.empty() || _protocol.empty() || _path.empty())
    {
        _code = 504;
        exitWithError("Missing informations");
        return 0;
    }
    if ((this->_method == "POST" && getBody().empty()))
    {
        _code = 405;
        exitWithError("Method not allowed");
        return 0;
    }
    if ((this->_method != "GET") && (this->_method != "POST") && (this->_method != "DELETE"))
    {
        _code = 501;
        exitWithError("Not allowed method");
        return 0;
    }
    if (this->_protocol != "HTTP/1.1")
    {
        _code = 504;
        exitWithError(" Wrong Protocol");
        return 0;
    }
    std::cout << GREEN << this->lines_body.size() << static_cast<std::string::size_type>(server.getClientMaxBody_s());
    if (this->lines_body.size() > static_cast<std::string::size_type>(server.getClientMaxBody_s()))
    {
        _code = 404;
        exitWithError(" Wrong Client Max");
        return 0;
    }
    else
    {
        _code = 200;
        return 1;
    }
}

std::string Request::getMethod()
{
    return _method;
}

std::string Request::getPath()
{
    return _path;
}

std::string Request::getBody()
{
    return lines_body;
}

std::string Request::getProtocol()
{
    return _protocol;
}

std::string Request::getHost()
{
    return _host;
}

std::string Request::getQuery()
{
    return _query;
}

int Request::getCode()
{
    return _code;
}

void Request::printMessage()
{

    std::cout << " ==== fullRquest ==== \n"
              << _fullRequest << std::endl
              << "=== END fullrequest ====" << std::endl;
}

std::string Request::getFullRequest(void)
{
    std::string temp = _fullRequest;
    std::size_t found;

    found = temp.find("\r\n\r\n");
    std::string new_request = temp.substr(found);
    return new_request;
}

void Request::setPath(std::string path)
{
    this->_path = path;
}


std::string Request::getContentType()
{
    return _contentType;
}

std::string Request::getContentLength()
{
    return _contentLength;
}

void Request::setContentType(std::string contentType)
{
    this->_contentType = contentType;
}
void Request::setContentLength(std::string contentLength)
{
    this->_contentLength = contentLength;
}
void Request::setCode(int code)
{
    this->_code = code;
}
/*
GET /css/styles.css HTTP/1.1
Host: localhost:8008
Connection: keep-alive
sec-ch-ua: "Not A(Brand";v="99", "Google Chrome";v="121", "Chromium";v="121"
sec-ch-ua-mobile: ?0
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36
sec-ch-ua-platform: "Linux"
*/
// Accept: text/css,*/*;q=0.1
/*
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: no-cors
Sec-Fetch-Dest: style
Referer: http://localhost:8008/register.html
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: en-US,en;q=0.9




POST /register.html HTTP/1.1
Host: localhost:8008
Connection: keep-alive
Content-Length: 70
Cache-Control: max-age=0
sec-ch-ua: "Not A(Brand";v="99", "Google Chrome";v="121", "Chromium";v="121"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Linux"
Upgrade-Insecure-Requests: 1
Origin: http://localhost:8008
Content-Type: application/x-www-form-urlencoded
User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/121.0.0.0 Safari/537.36
*/
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
/*
Sec-Fetch-Site: same-origin
Sec-Fetch-Mode: navigate
Sec-Fetch-User: ?1
Sec-Fetch-Dest: document
Referer: http://localhost:8008/register.html
Accept-Encoding: gzip, deflate, br, zstd
Accept-Language: en-US,en;q=0.9

std::string name = "name=alexandre&dob=1993-09-12&email=blahblah%40gmail.com&password=1234"
*/
