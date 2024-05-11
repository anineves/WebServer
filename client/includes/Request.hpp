#ifndef REQUEST_HPP
#define REQUEST_HPP

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
# include "./Response.hpp"


class Request{
public:
    Request(std::string request);
    Request();
    ~Request();
    std::string getMethod();
	std::string getPath();
    std::string getProtocol();
    int         getCode();
    std::string getContentType();
    std::string getContentLength();
    std::string getHost();
    std::string getQuery();
    void printMessage();
    void setPath( std::string path);
    void verifyLocations(Server server);
    void setContentType(std::string contentType);
    void setContentLength(std::string contentLength);
    std::string getBody();
    void setCode(int code);
    Server  r_server;
    std::string lines_body;
    std::string _query;
    std::string getFullRequest(void);
    bool verific_errors(Server server);
    bool has_header;
    void parserHeader(std::string header);
    std::string _fullRequest;
    void parserBody(std::string body);

private:
    std::string _method;
	std::string _protocol;
	std::string _path;
    std::string _contentType;
    std::string _contentLength;
    std::string _host;
    std::map<std::string, std::string> lines_header;
    int         _code;
};

#endif
