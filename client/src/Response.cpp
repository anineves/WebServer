#include "../includes/Response.hpp"

Response::Response()
{

}

Response::Response(Server server)
{
    m_server = server;
}

Response::~Response()
{

}

std::string Response::buildErrorResponse(int code)
{
    std::string response;
    std::stringstream stringCode;
    stringCode << code;
    response = "HTTP/1.1 " + stringCode.str() + " " + responseStatus(code) +"\r\n";
    response += "Content-Type: Content-Type: text/html \r\n\r\n";
    return response;
}


std::string Response::buildResponse(Request request) {
    std::string response;
    if (request.getMethod() == "GET" || request.getMethod() == "POST" || request.getMethod() == "DELETE") 
    {
        //std::cout << "valor redirect " << m_server.getredirect << std::endl;
        std::string filePath = obtainFilePath(request.getPath());
        std::ifstream file(filePath.c_str());
        if (file) {
            std::stringstream content;
            content << file.rdbuf();
            std::string fileContent = content.str();

            std::stringstream length;
            length << fileContent.size();
            std::stringstream stringCode;
            stringCode << request.getCode();
            response = "HTTP/1.1 " + stringCode.str() + " " + responseStatus(request.getCode()) +"\r\n";
            response += "Content-Length: " + length.str() + "\r\n";
            response += "Content-Type: " + getContentType(filePath) + "\r\n\r\n";
            //std::cout << "response " << response << std::endl;
            
            response += fileContent;
        } else {
            response = "HTTP/1.1 404 Not Found\r\n\r\n";
        }
    } else {
        response = "HTTP/1.1 501 Not Implemented\r\n\r\n";
    }

    return response;
}

std::string Response::obtainFilePath(const std::string& request) {
    size_t start = request.find(" ") + 1;
    size_t end = request.find(" ", start);
    std::string path = request.substr(start, end - start);
    if (path == "/") {
        path = "/index.html";
    }
    std::string fullPath;
    if(path.find(".html") != std::string::npos)
        fullPath = m_server.getRoot_s() + "/html" +  path;
    else
        fullPath = m_server.getRoot_s() + path;
    if (!isValidPath(fullPath)) {
        fullPath = "frontend/error/404.html";
    }

    return fullPath;
}

bool Response::isValidPath(std::string& fullPath) {
    std::ifstream file(fullPath.c_str());
    return file.good();
}






std::string Response::getContentType(const std::string& filePath) {
    size_t ext = filePath.find_last_of(".");
    std::string extension = "";
    if (ext != std::string::npos) {
        extension = filePath.substr(ext + 1);
    }
    if (extension == "html") {
        return "text/html";
    } else if (extension == "css") {
        return "text/css";
    } else if (extension == "ico") {
        return "image/vnd.microsoft.icon";
    } else if (extension == "txt") {
        return "text/plain";
     } else if (extension == "svg") {
        return "image/svg+xml";
    } else if (extension == "jpg" || extension == "jpeg") {
        return "image/jpeg";
    } else if (extension == "png") {
        return "image/png";
    } else if (extension == "gif") {
        return "image/gif";
    } else if (extension == "pdf") {
        return "application/pdf";
    } else if (extension == "x-www-form-urlenconded") {
        return "application/x-www-form-urlencoded";
    }else if (extension == "py") {
        return "text/x-python";
    } else {
        return "application/octet-stream";
    }
}

std::string Response::responseStatus (int code)
{
    switch (code) {
        case 100:
            return "Continue";
        case 101:
            return "Switching Protocols";
        case 102:
            return "Processing";
        case 103:
            return "Early Hints";
        case 200:
            return "OKi";
        case 201:
            return "Created";
        case 202:
            return "Accepted";
        case 203:
            return "Non-Authoritative Information";
        case 204:
            return "No Content";
        case 205:
            return "Reset Content";
        case 206:
            return "Partial Content";
        case 300:
            return "Multiple Choice";
        case 301:
            return "Moved Permanently";
        case 302:
            return "Found";
        case 303:
            return "See Other";
        case 304:
            return "Not Modified";
        case 307:
            return "Temporary Redirect";
        case 308:
            return "Permanent Redirect";
        case 400:
            return "Bad Request";
        case 401:
            return "Unauthorized";
        case 403:
            return "Forbidden";
        case 404:
            return "Not Found";
        case 405:
            return "Method Not Allowed";
        case 406:
            return "Not Acceptable";
        case 407:
            return "Proxy Authentication Required";
        case 408:
            return "Request Timeout";
        case 409:
            return "Conflict";
        case 410:
            return "Gone";
        case 411:
            return "Length Required";
        case 412:
            return "Precondition Failed";
        case 413:
            return "Payload Too Large";
        case 414:
            return "URI Too Long";
        case 415:
            return "Unsupported Media Type";
        case 416:
            return "Requested Range Not Satisfiable";
        case 417:
            return "Expectation Failed";
        case 418:
            return "I'm a teapot";
        case 421:
            return "Misdirected Request";
        case 425:
            return "Too Early";
        case 426:
            return "Upgrade Required";
        case 428:
            return "Precondition Required";
        case 429:
            return "Too Many Requests";
        case 431:
            return "Request Header Fields Too Large";
        case 451:
            return "Unavailable for Legal Reasons";
        case 500:
            return "Internal Server Error";
        case 501:
            return "Not Implemented";
        case 502:
            return "Bad Gateway";
        case 503:
            return "Service Unavailable";
        case 504:
            return "Gateway Timeout";
        case 505:
            return "HTTP Version Not Supported";
        case 506:
            return "Variant Also Negotiates";
        case 507:
            return "Insufficient Storage";
        case 510:
            return "Not Extended";
        case 511:
            return "Network Authentication Required";
        default:
            return "Unknown HTTP status code";
        };
}