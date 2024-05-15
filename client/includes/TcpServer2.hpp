#ifndef TCPSERVER_HPP
# define TCPSERVER_HPP

# include <iostream>
# include <sstream>
# include <fstream> 
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
# include <dirent.h>
# include <map>
# include <signal.h>
# define TIMEOUT 20


class Connection;
class Request;

class TcpServer2 {
private:
    std::vector<Server>             m_server;
    std::vector<struct sockaddr_in> m_addresses;
    std::vector<int>                m_sockets;
    int                             epoll_fd;
    std::map<int, time_t>           socketCreation;
    std::string                     _client_request;
    std::string                     _header;
    std::string                     _body;

public:
    TcpServer2(std::vector<Server> server);
    ~TcpServer2();

    std::map<int, std::string> responseMap;
    std::map<int, Server*> clientServerMap;
    
    std::string clientRequest;
    void startListen();
    void printPorts();
    int getEpoll();
    std::vector<int> getAllSockets();
    void verificTimeOut();
    //struct epoll_event          m_events[MAX_EVENTS];


    void startServer();
    void acceptNewConnection(epoll_event &event, int fd, int j);
    void handleInput(epoll_event &event, int fd); 
    void handleOutput(epoll_event &event, int fd);
    void showClientHeader(struct epoll_event &m_events, Request &request1);
    void sendResponse(int client_socket, const std::string& response);
    void closeServer();
    uint32_t strToNet(const std::string &ip_address);

    void setAddresses();
    void closeConnection();

};

#endif
