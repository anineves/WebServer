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
    ~Request();
    std::string getMethod();
	std::string getPath();
    std::string getProtocol();
    int         getCode();
    void printMessage();
    void setPath( std::string path);
    void verifyLocations(Server server);

private:
    std::string _fullRequest;
    std::string _method;
	std::string _protocol;
	std::string _path;
    std::map<std::string, std::string> lines_header;
    std::map<std::string, std::string> lines_body;
    int         _code;
    void parser(std::string header);
    void verific_errors();
};

#endif