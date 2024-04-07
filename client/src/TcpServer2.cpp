#include "../includes/TcpServer2.hpp"

TcpServer2::TcpServer2(std::vector<Server> servers) : m_server(servers){
    

    setAddresses();
    startServer();
    startListen();
    
}

TcpServer2::~TcpServer2() {
    this->closeServer();
    std::cout << "TcpServer Destructor called.\n";
}



void TcpServer2::startServer() {
    std::vector<struct sockaddr_in>::iterator it;

     std::cout << "@#$@#@$##@$$# Tamanho addresses :" << m_addresses.size() << std::endl;;
    for (it = m_addresses.begin(); it != m_addresses.end(); it++) {
        
        int curr_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (curr_socket < 0) {
            exitWithError("Socket creation failed");
            exit(EXIT_FAILURE);
        }
        int optval = 1;
        if (setsockopt(curr_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0) {
            exitWithError("Not Reusing Socket");
            exit (EXIT_FAILURE);
        }
        if (bind(curr_socket, (struct sockaddr *)&*it, sizeof(*it)) < 0) {
            exitWithError("Bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(curr_socket,20) < 0) {
            exitWithError("Socket listen Failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "\n\n * * * Listening Server at following ports * * *   \n\n";
        std::cout << convert_uint32_to_str(ntohl(it->sin_addr.s_addr)) << " : sockfd " << curr_socket << std::endl;
        this->m_sockets.push_back(curr_socket);
    }

}   

void TcpServer2::startListen() {
    
    this->epoll_fd = epoll_create(MAXEPOLLSIZE);
    if(epoll_fd == -1)
    {
        exitWithError("Create epoll");
        exit(EXIT_FAILURE);
    }

    struct epoll_event          m_event;
    ft_memset(&m_event, 0,sizeof(m_event));

    struct epoll_event          m_event_list[MAX_EVENTS];
    
    std::vector<int>::iterator it2;
    for (it2 = this->m_sockets.begin(); it2 != this->m_sockets.end(); ++it2) {
        int add;
        m_event.events = EPOLLIN;
        m_event.data.fd = *it2;

        add = epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, *it2, &m_event);
        if (add == -1) {
            exitWithError("failed to add to epoll");
            exit(EXIT_FAILURE);
        }
    }



    while(1) {
        int num_events = epoll_wait(epoll_fd, m_event_list, MAXEPOLLSIZE, 2000);
        if (num_events == -1) {
            exitWithError("Epoll wait");
            continue ;
        }
        std::cout << "num_events= "<< num_events << std::endl;
        for (int i = 0; i < num_events; i++) {
            std::cout << "ENTREI ===222222222============= \n";
            for (size_t j = 0; j < this->m_server.size(); ++j) {
                std::cout << "ENTREI ======================= \n";
                struct sockaddr_in addr;
                socklen_t addr_len = sizeof(addr);
                    std::cout << "entrei antes  valor socket" <<   m_sockets[j] << std::endl;
                if (m_event_list[i].data.fd == m_sockets[j]) {
                    m_server[j].setSocketAddr_len(sizeof(m_sockets[j]));
                    std::cout << "entrei accept " << std::endl;
                    int client_socket = accept(m_event_list[i].data.fd, (struct sockaddr *)&addr, &addr_len);
                    std::cout << "entrei accept " << std::endl;
                    if (client_socket == -1) {
                        exitWithError("Can't Accept something");
                        continue;
                    }
                    std::cout << "New Connection on port: " << ntohs(m_server[j].getSocketAddr().sin_port) << std::endl;
             
                    m_event.events = EPOLLIN  | EPOLLRDHUP;
                    m_event.data.fd = client_socket;
                    if (epoll_ctl(this->getEpoll(), EPOLL_CTL_ADD, client_socket, &m_event) == -1) {
                        exitWithError("Problems in Epoll_CTL.");
                        exit(EXIT_FAILURE);
                    }
                    //Associar cadaa socket a cada client
                    clientServerMap[client_socket] = &m_server[j];
                } 
                else if (m_event_list[i].events & EPOLLIN)
                {
                    //if(m_server[j].getPort_s() == 8008)
                    Server* server = clientServerMap[m_event_list[i].data.fd];
                    if (server != NULL) {
                        std::string clientRequest = showClientHeader(m_event_list[i]);
                        Request request(clientRequest);
                        std::cout << "Path from request = " << request.getPath() << std::endl;
                        request.verifyLocations(*server);
                        Response response(*server);
                        std::string serverResponse = response.buildResponse(request);
                        m_event_list[i].events = EPOLLOUT;
                        epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, m_event_list[i].data.fd, &m_event_list[i]);
                        responseMap[m_event_list[i].data.fd] = serverResponse;
                    }
                   
                }
                else if (m_event_list[i].events & EPOLLOUT)
                {
                    std::string serverResponse = responseMap[m_event_list[i].data.fd];
                    sendResponse(m_event_list[i].data.fd, serverResponse);
                    responseMap.erase(m_event_list[i].data.fd);
                    m_event_list[i].events = EPOLLIN;
                    epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, m_event_list[i].data.fd, &m_event_list[i]);
        
                }
            }
        }
    }


}

std::string TcpServer2::showClientHeader(struct epoll_event &m_events) {
    char buffer[2048]; 
    int bytesReceived = recv(m_events.data.fd, buffer, sizeof(buffer), 0);
    if (bytesReceived < 0) {
        log("Error receiving data from client");
        return "";
    }

    buffer[bytesReceived] = '\0';
    //log("Header from Client:");
    //log(buffer);
    return std::string(buffer);
}

void TcpServer2::sendResponse(int client_socket, const std::string& response){
    //log("Server Response Header:");
    //log(response);

    long bytesSent = send(client_socket, response.c_str(), response.size(), 0);
    if (bytesSent == (long)response.size()) {
        log("------ Server Response sent to client ------\n\n");
    } else {
        log("Error sending response to client");
    }
}

void TcpServer2::closeServer() {
    std::cout << "closeServer Function called.\n";
    exit(0);
}

uint32_t TcpServer2::strToNet(const std::string &ip_address) {
    struct in_addr addr;
    if (inet_pton(AF_INET, ip_address.c_str(), &addr) <= 0) {
        std::cerr << "Error converting IP address" << std::endl;
        return 0;
    }
    return htonl(addr.s_addr);
}

void TcpServer2::printPorts() {
    for (size_t i = 0; i < this->m_server.size(); i++) {
        std::cout << this->m_server[i].getPort_s() << " ";
    }
    std::cout << " * * * \n\n";
}



// UTILS 

int TcpServer2::getEpoll() {
    return this->epoll_fd;
}

void TcpServer2::setAddresses () {
    for( size_t i = 0; i < m_server.size(); i++)
    {
        memset(&m_server[i].s_socketAddress, 0, sizeof(m_server[i].s_socketAddress));
        m_server[i].s_socketAddress.sin_family = AF_INET;
        m_server[i].s_socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
        std::cout << "valor Porta htons: " << htons(m_server[i].getPort_s()) << std::endl;
        std::cout << "valor Porta: " << m_server[i].getPort_s() << std::endl;
        m_server[i].s_socketAddress.sin_port = htons(m_server[i].getPort_s());
        m_addresses.push_back(m_server[i].getSocketAddr());
    }

    std::cout << "m_adresses size = " << m_addresses.size() << std::endl;
}
