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
    // GET methods
    std::string getMethod();
	std::string getPath();
    std::string getProtocol();
    std::string getContentType();
    std::string getContentLength();
    std::string getHost();
    std::string getQuery();
    std::string getBody();
    std::string getFullRequest(void);
    int         getCode();
    // SET methods
    void        setContentType(std::string contentType);
    void        setContentLength(std::string contentLength);
    void        setCode(int code);
    void        setPath( std::string path);
    void        printMessage();
    void        verifyLocations(Server server);
    bool        verific_errors(Server server);
    void        parser(std::string header);
    bool        has_header;
    std::string lines_body;
    std::string _query;
    std::string _fullRequest;
    Server      r_server;

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
