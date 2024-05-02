#include "../includes/Request.hpp"

Request::Request(std::string request) : _fullRequest(request)
{
    parser(_fullRequest);
    _code = 200;
}

Request::Request() {}

Request::~Request() {}

void Request::parser(std::string header)
{

    //std::cout << "Header " << header << std::endl;
    std::istringstream iss(header);
    std::stringstream ss(header);
    std::string line;

    iss >> _method;
    iss >> _path;
    iss >> _protocol;

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
                std::cout << MAGENTA << "CONTENT LENGTH :" << _contentLength << RESET << std::endl;
            }
            if(fword == "Content-Type:")
            {
                _contentType = sword;
                std::cout << MAGENTA << "CONTENT Type :" << _contentType << RESET << std::endl;
            }
    }
    while (std::getline(ss, line) && line != "\r")
    {
        // std::cout << "linha:::::" << line << std::endl;
        if (line.find('=') != std::string::npos)
        {
            std::string name(line.substr(0, line.find('=')));
            std::string content(line.substr(line.find('=') + 1, line.find('\n')));
            if (content.length() != 0)
            {

                this->lines_body  += line;
            }
        }

    }
    
    std::cout << MAGENTA << "LINHAS " << this->lines_body << RESET << std::endl;

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
    // Aqui Depois em vez do exitWithError colocar os erros, por exemplo se o metodo for diferente do esperado e o erro 501
    if (_method.empty() || _protocol.empty() || _path.empty())
    {
        _code = 504;
        exitWithError("Missing informations");
    }
    if ((this->_method != "GET") && (this->_method != "POST") && (this->_method != "DELETE"))
    {
        _code = 501;
        exitWithError("Not allowed method");
    }
    if (this->_protocol != "HTTP/1.1")
    {
        _code = 504;
        exitWithError(" Wrong Protocol");
    }
    else
    {
        _code = 200;
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
    return new_request ;
}

void Request::setPath(std::string path)
{
    this->_path = path;
}

void Request::verifyLocations(Server server)
{
/*     std::vector<Location> tmp = server.getLocations();
    for (size_t i = 0; i < tmp.size(); i++) {
        tmp[i].printLoc();
    } */
    std::string pathRequest = this->_path;

    std::string bestMatchPath;
    //size_t bestMatchLength = 0;


    size_t extensionPos;
    int location_found = 0;
    std::vector<Location> locationStack = server.getLocations(); 
    while (!location_found) {
        for (size_t i = 0; i < locationStack.size(); i++) {
            if (pathRequest.compare(locationStack[i].getPath()) == 0) {
                bestMatchPath = locationStack[i].getPath();
                location_found++;
            }
            std::cout << "-------------------pathRequest: " << pathRequest << std::endl;
            std::cout << "-------------------locationPathComparing: " << locationStack[i].getPath() << std::endl;
        }
        if (!location_found) {
            extensionPos = pathRequest.find_last_of('/');
            if (extensionPos != std::string::npos) {
                pathRequest = pathRequest.substr(0, extensionPos);
            } else {
                bestMatchPath = "/";
                location_found++;
            }
        }
    }
    std::cout << RED << "BESTMATCHPATH = " << RESET << bestMatchPath << std::endl;


/*     for (size_t i = 0; i < server.getLocations().size(); ++i) {
        Location location = server.getLocations()[i];
        std::string locationPath = location.getPath();
        
        // Remova a extensão do caminho da localização para a comparação
        size_t extensionPos = locationPath.find_last_of('.');
        std::string locationBasePath = (extensionPos != std::string::npos) ? locationPath.substr(0, extensionPos) : locationPath;

        if (pathRequest.find(locationBasePath) == 0 && 
            (pathRequest.size() == locationBasePath.size() || pathRequest[locationBasePath.size()] == '/')) {
            bestMatchPath = locationPath;
            //bestMatchLength = locationPath.size();
        }
    } */
     
    std::cout << std::endl;
    // std::cout << server.getLocations()[1].getAllowMethods() << std::endl;
    if (!bestMatchPath.empty())
    {
        //Connection connect;
        std::cout << "\n\n###############- Path Location " << bestMatchPath << " Path request " << pathRequest << std::endl;

        for (size_t i = 0; i < server.getLocations().size(); ++i)
        {
            Location location = server.getLocations()[i];
             if (location.getPath() == bestMatchPath) {
              
                std::cout << "\n\n\n @@@@@entrei Location :"  << location.getPath() << "\n\n" << std::endl;
                if (!location.getUploadTo().empty())
                    server.setUploadTo(location.getUploadTo());
                if (!location.getCgiPath().empty())
                    server.setCgiPath(location.getCgiPath());
                if (!location.getCgiExt().empty()) {
                    server.setExecutable("true");
                    std::cout << "\n\n### entrei Locations Executable" << std::endl;
                    server.setCgiPath(location.getCgiPath());
                }
                if (!location.getAutoIndex().empty())
                    server.setAutoIndex(location.getAutoIndex());
                if (!location.getAllowMethods().empty()) {
                     server._methods.clear();
                    server.setMethods(location.getAllowMethods());
                }
                if (!location.getReturn().empty()) {
                    server.setRedirect("true");
                    //server.setIndex_s(location.getReturn());
                }
                break;
            }
        }
    }
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