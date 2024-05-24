#ifndef RESPONSE_HPP
#define RESPONSE_HPP

# include <iostream>
# include <sstream>
#include <fstream> 
# include <string>
# include <vector>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <poll.h>
# include  <map>
# include "./ConfigFile.hpp"
# include "./webServer.hpp"
# include "./Utils.hpp"
# include "./Server.hpp"
# include "./TcpServer2.hpp"
# include "./Request.hpp"
# include "./Cgi.hpp"

class Request;

class Response {
public:
    std::map<std::string, std::string> _default_error;


    Response();
    Response(Server &server);
    ~Response();
    std::string     obtainFilePath(const std::string &request, Location &location);
    std::string     getContentType(const std::string& filePath);
    bool            isValidPath(std::string& fullPath);
    std::string     responseStatus (int code);
    std::string     buildResponse(Request request, Location &location);
    std::string     buildErrorResponse(int code);
    std::string     getDefaultError(std::string code);
    void            printDefaultErrors();

    Server          m_server; 
private:
};

#endif