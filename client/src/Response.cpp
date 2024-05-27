#include "../includes/Response.hpp"

Response::Response()
{
}

Response::Response(Server &server) : m_server(server)
{
    m_server = server;
    _default_error["413"] = "./errorpages/erro413.html";
    _default_error["400"] = "./errorpages/erro400.html";
    _default_error["401"] = "./errorpages/erro401.html";
    _default_error["403"] = "./errorpages/erro403.html";
    _default_error["404"] = "./errorpages/erro404.html";
    _default_error["405"] = "./errorpages/erro405.html";
    _default_error["406"] = "./errorpages/erro406.html";
    _default_error["407"] = "./errorpages/erro407.html";
    _default_error["408"] = "./errorpages/erro408.html";
    _default_error["409"] = "./errorpages/erro409.html";
    _default_error["410"] = "./errorpages/erro410.html";
    _default_error["411"] = "./errorpages/erro411.html";
    _default_error["412"] = "./errorpages/erro412.html";
    _default_error["413"] = "./errorpages/erro413.html";
    _default_error["414"] = "./errorpages/erro414.html";
    _default_error["500"] = "./errorpages/erro500.html";
    _default_error["501"] = "./errorpages/erro501.html";
    _default_error["502"] = "./errorpages/erro402.html";
    _default_error["503"] = "./errorpages/erro503.html";
    _default_error["504"] = "./errorpages/erro504.html";
    _default_error["505"] = "./errorpages/erro505.html";
    _default_error["506"] = "./errorpages/erro506.html";
    _default_error["507"] = "./errorpages/erro507.html";
    _default_error["510"] = "./errorpages/erro510.html";
    _default_error["511"] = "./errorpages/erro511.html";
}

Response::~Response()
{
}

std::string Response::buildErrorResponse(int code)
{
    std::string response;

    std::stringstream ss;
    ss << code;
    std::string codestr = ss.str();
    std::ifstream file(getDefaultError(codestr).c_str());
    if (file)
    {
        std::stringstream content;
        content << file.rdbuf();
        std::string fileContent = content.str();
        std::stringstream length;
        length << fileContent.size();
        std::stringstream stringCode;
        stringCode << code;
        response = "HTTP/1.1 " + stringCode.str() + " " + responseStatus(code) + "\r\n";
        response += "Content-Length: " + length.str() + "\r\n";
        response += "Content-Type: " + getContentType(getDefaultError(codestr).c_str()) + "\r\n\r\n";

        response += fileContent;
    
    }
    else {
    response = "HTTP/1.1 " + codestr + " " + responseStatus(code) + "\r\n";
    response += "Content-Type: text/html\r\n\r\n";
    std::string fileContent = "<!DOCTYPE html>\n"
                              "<html lang=\"en\">\n"
                              "<head>\n"
                              "    <meta charset=\"UTF-8\">\n"
                              "    <link rel=\"stylesheet\" href=\"error.css\">\n"
                              "    <title>Oops! Page Not Found</title>\n"
                              "</head>\n"
                              "<body>\n"
                              "    <header>\n"
                              "        <h1>404</h1>\n"
                              "        <p>Oops! Page not found </p>\n"
                              "    </header>\n"
                              "</body>\n"
                              "</html>";
    response += fileContent;
}
    return response;
}

std::string Response::buildResponse(Request request, Location &location)
{
    std::string response;
        std::string filePath = obtainFilePath(request, location);
        std::ifstream file(filePath.c_str());
        if (file)
        {
            std::stringstream content;
            content << file.rdbuf();
            std::string fileContent = content.str();
            std::stringstream length;
            length << fileContent.size();
            std::stringstream stringCode;
            stringCode << request.getCode();
            response = "HTTP/1.1 " + stringCode.str() + " " + responseStatus(request.getCode()) + "\r\n";
            response += "Content-Length: " + length.str() + "\r\n";
            response += "Content-Type: " + getContentType(filePath) + "\r\n\r\n";

            response += fileContent;
        }
        else
        {
            response = "HTTP/1.1 404 Not Found\r\n\r\n";
        }

    return response;
}

std::string Response::obtainFilePath(Request &request, Location &location)
{

    std::string path = request.getPath();
    if (path == "/")
    {
        path = "/index.html";
    }
    std::string fullPath;
    if (path.find(".html") != std::string::npos)
        fullPath = location.getRoot() + "/html" + path;
    else
        fullPath = location.getRoot() + path;
    if (!isValidPath(fullPath))
    {
        if(request.getMethod() !=  "DELETE")
            request.setCode(404);
        fullPath = m_server.getErrorPage_s();
        if(!isValidPath(fullPath))
        {
            fullPath = getDefaultError("404");
        }
    }

    return fullPath;
}

bool Response::isValidPath(std::string &fullPath)
{
    std::ifstream file(fullPath.c_str());
    return file.good();
}

std::string Response::getContentType(const std::string &filePath)
{
    size_t ext = filePath.find_last_of(".");
    std::string extension = "";
    if (ext != std::string::npos)
    {
        extension = filePath.substr(ext + 1);
    }
    if (extension == "html")
    {
        return "text/html";
    }
    else if (extension == "css")
    {
        return "text/css";
    }
    else if (extension == "ico")
    {
        return "image/vnd.microsoft.icon";
    }
    else if (extension == "txt")
    {
        return "text/plain";
    }
    else if (extension == "svg")
    {
        return "image/svg+xml";
    }
    else if (extension == "jpg" || extension == "jpeg")
    {
        return "image/jpeg";
    }
    else if (extension == "png")
    {
        return "image/png";
    }
    else if (extension == "gif")
    {
        return "image/gif";
    }
    else if (extension == "pdf")
    {
        return "application/pdf";
    }
    else if (extension == "x-www-form-urlenconded")
    {
        return "application/x-www-form-urlencoded";
    }
    else if (extension == "py")
    {
        return "text/x-python";
    }
    else
    {
        return "application/octet-stream";
    }
}

std::string Response::responseStatus(int code)
{
    switch (code)
    {
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

std::string Response::getDefaultError(std::string code)
{

    // printDefaultErrors();
    std::map<std::string, std::string>::iterator it;

    for (it = this->_default_error.begin(); it != this->_default_error.end(); it++)
    {
        if (it->first == code)
        {
            return it->second;
        }
    }

    return "";
}

void Response::printDefaultErrors()
{
    std::cout << "Conteúdo de _default_error:" << std::endl;
    for (std::map<std::string, std::string>::const_iterator it = _default_error.begin(); it != _default_error.end(); ++it)
    {
        std::cout << "Código: " << it->first << " - Caminho: " << it->second << std::endl;
    }
}