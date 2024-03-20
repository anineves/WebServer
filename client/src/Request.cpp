#include "../includes/Request.hpp"

Request::Request(std::string request) : _fullRequest(request)
{
    parser(_fullRequest);
    _code = 200;
}

Request::~Request()
{

}

void Request::parser(std::string header)
{

    std::cout << "Header " << header << std::endl;
    std::istringstream iss(header);
    std::stringstream ss(header);
    std::string line;
    
    iss >> _method;
    iss >> _path; 
    iss >> _protocol;
    
     while (std::getline(ss, line) && line != "\r") {
        //std::cout << "linha:::::" << line << std::endl;
        if (line.find(':') != std::string::npos) {
            std::string name(line.substr(0, line.find(':')));
            std::string content(line.substr(line.find(':') + 2, line.find('\n')));
            if (content.length() != 0) {
        
                this->lines_header[name] = content;
            }
        }
    }
    while (std::getline(ss, line) && line != "\r") {
        //std::cout << "linha:::::" << line << std::endl;
        if (line.find('=') != std::string::npos) {
            std::string name(line.substr(0, line.find('=')));
            std::string content(line.substr(line.find('=') + 1, line.find('\n')));
            if (content.length() != 0) {
        
                this->lines_body[name] = content;
            }
        }
    }

    verific_errors();
    /* std::map<std::string, std::string>::const_iterator it;
     std::cout << "Maps Header:::::" << std::endl;
    for (it = lines_header.begin(); it != lines_header.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }*/
    /*std::map<std::string, std::string>::const_iterator it;
     std::cout << "Maps Location:::::" << std::endl;
    for (it = lines_body.begin(); it != lines_body.end(); ++it) {
        std::cout << it->first << ": " << it->second << std::endl;
    }*/
}

void Request::verific_errors()
{
    //Aqui Depois em vez do exitWithError colocar os erros, por exemplo se o metodo for diferente do esperado e o erro 501
    if (_method.empty() || _protocol.empty() || _path.empty()) {
        _code = 504;
        exitWithError("Missing informations");
    }
    if((this->_method !=  "GET") && (this->_method != "POST") && (this->_method != "DELETE")) {
        _code = 501;
        exitWithError("Not allowed method");
    }
    if(this->_protocol != "HTTP/1.1") {
        _code = 504;
        exitWithError(" Wrong Protocol");
    }
    else {
        _code = 200;
    }
    //se o method for POST (length > 0)
    //content-length? == 0 // this method = "POST" and length == 0 == code 504
    //content-type?
}

std::string Request::getMethod()
{
	return _method;
}

std::string Request::getPath()
{
	return _path;
}

std::string Request::getProtocol()
{
	return _protocol;
}

int Request::getCode() 
{
    return _code;
}

void    Request::printMessage() {
    
    std::cout << " ==== fullRquest ==== \n" << _fullRequest << std::endl << "=== END fullrequest ====" << std::endl; 
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
//Accept: text/css,*/*;q=0.1
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
//Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
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