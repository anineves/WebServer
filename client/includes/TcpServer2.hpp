#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# include <iostream>
# include <sstream>
#include <fstream> 
# include <string>
# include <vector>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <sys/epoll.h>
# include "./ConfigFile.hpp"
# include "./webServer.hpp"
# include "./Utils.hpp"
# include "./Server.hpp"
# include "./Request.hpp"
# include "./Response.hpp"


class Request; 

class TcpServer2 {
public:
    TcpServer2(std::vector<Server> server);
    ~TcpServer2();

    void startListen(Server &server);
    void printPorts();

private:
    std::vector<Server>         m_server;
    int                         epoll_fd;
    struct epoll_event          m_event;
    struct epoll_event          m_events[MAX_EVENTS];

    void startServer();
    void acceptConnection();
    std::string showClientHeader(int client_socket);
    void sendResponse(int client_socket, const std::string& response);
    void closeServer();
    uint32_t strToNet(const std::string &ip_address);
};

#endif