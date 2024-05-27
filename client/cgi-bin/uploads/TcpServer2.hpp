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
# include <algorithm>
# define TIMEOUT 10

class Connection;
class Request;

class TcpServer2 {
private:
    std::vector<Server>                 m_server;
    std::vector<struct sockaddr_in>     m_addresses;
    std::vector<int>                    m_sockets;
    int                                 epoll_fd;
    std::map<int, time_t>               socketCreation;
    std::string                         _client_request;
    std::string                         _body;
    std::string                         _bodypart;
    size_t                              _max_length;
    size_t                              _my_bytes;
    size_t                              _chunk_size;
    bool                                _fullheader;
    bool                                _has_header;
    std::string                         _header;
    bool                                _chunked;
    bool                                _first_chunk;
    bool                                _last_chunk;

public:
    TcpServer2(std::vector<Server> &server);
    ~TcpServer2();
    //ShowCLienteHeader()
    std::string                         chunk;
    std::string                         chunk_length_str;

    bool                                timeout;
    std::map<int, std::string>          responseMap;
    std::map<int, Server*>              clientServerMap;
    std::string                         clientRequest;
    void                                startListen();
    void                                printPorts();
    int                                 getEpoll();
    void                                verificTimeOut();
    void                                startServer();
    void                                acceptNewConnection(epoll_event &event, int fd, int j);
    void                                handleInput(epoll_event &event, int fd); 
    void                                handleOutput(epoll_event &event, int fd);
    void                                showClientHeader(struct epoll_event &m_events, Request &request1);
    void                                sendResponse(int client_socket, const std::string& response);
    void                                closeServer();
    uint32_t                            strToNet(const std::string &ip_address);
    void                                setAddresses();
    void                                closeConnection();
    int                                 handleChunkedRequest(size_t found_header, Request &request);
    void                                fetchAndParseRequest(Request &request);

};

#endif