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
# include <poll.h>
# include "./ConfigFile.hpp"
# include "./webServer.hpp"
# include "./Utils.hpp"
# include "./Server.hpp"
# include "./Request.hpp"
# include "./Response.hpp"


class Request; 

class TcpServer2 {
public:
    TcpServer2(Server& server);
    ~TcpServer2();

    void startListen();

private:
    int m_socket;
    struct sockaddr_in m_socketAddress;
    socklen_t m_socketAddress_len;
    std::string m_serverMessage;
    Server m_server;
    struct pollfd m_pollFds[1];

    int startServer();
    void acceptConnection();
    std::string showClientHeader(int client_socket);
    void sendResponse(int client_socket, const std::string& response);
    void closeServer();
    uint32_t strToNet(const std::string &ip_address);
    
   

};

#endif