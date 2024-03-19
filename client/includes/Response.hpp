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


class Request;

class Response {
public:
    Response();
    Response(Server server);
    ~Response();
     std::string obtainFilePath(const std::string& request);
    std::string getContentType(const std::string& filePath);
    bool isValidPath(std::string& fullPath);
    std::string responseStatus (int code);
    std::string buildResponse(Request request);
    Server m_server; 

private:
};

#endif