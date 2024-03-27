#include "../includes/TcpServer2.hpp"

TcpServer2::TcpServer2(std::vector<Server> servers) : m_server(servers){
    
/*     if (this->startServer() != 0) {
        std::ostringstream ss;
        ss << "Failed to start server with Port: " ;
        log(ss.str());
    } */
    epoll_fd = epoll_create(100);
    if(epoll_fd == -1)
    {
        exitWithError("Create epoll");
        exit(EXIT_FAILURE);
    }
}

TcpServer2::~TcpServer2() {
    this->closeServer();
    std::cout << "TcpServer Destructor called.\n";
}

void TcpServer2::startServer() {

    for (int i = 0; i < MAX_CLIENTS; ++i) {

        m_server[i].setSocket(socket(AF_INET, SOCK_STREAM, 0));
        if (socket(AF_INET, SOCK_STREAM, 0) == -1) {
            exitWithError("Socket creation failed");
            exit(EXIT_FAILURE);
        }

        memset(&m_server[i].s_socketAddress, 0, sizeof(m_server[i].s_socketAddress));
        m_server[i].s_socketAddress.sin_family = AF_INET;
        m_server[i].s_socketAddress.sin_addr.s_addr = INADDR_ANY;
        m_server[i].s_socketAddress.sin_port = htons(m_server[i].getPort_s());


        if (bind(m_server[i].getSocket(), (struct sockaddr *)&m_server[i].s_socketAddress, sizeof(m_server[i].s_socketAddress)) < 0) {
            exitWithError("Bind failed");
            exit(EXIT_FAILURE);
        }
        //startListen(m_server[i]);
        if (listen(m_server[i].getSocket(), 20) < 0) {
            exitWithError("Socket listen Failed");
            exit(EXIT_FAILURE);
        }
        m_event.events = EPOLLIN;
        m_event.data.fd = m_server[i].getSocket();
        if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_server[i].getSocket(), &m_event) == -1) {
            exitWithError("epoll_ctl: listen_sock");
            exit(EXIT_FAILURE);
        }
    }
    
    std::cout << "\n\n * * * Listening Server at folloiwing ports: ";
    printPorts();
    std::cout << std::endl;
}

/*void TcpServer2::startListen(Server &server) {
    //correr os servers e o Epoll eu todos ate ficarem ready (POLLIN || POLLOUT)


    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << " Port: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
    log(ss.str());

    //m_pollFds[0].fd = m_socket;
    //m_pollFds[0].events = POLLIN;
    
    while (true) {
        log("========== Waiting for a new connection =========\n\n\n");
        int pollResult = poll(m_pollFds, 1, -1);
        if (pollResult < 0) {
            exitWithError("Poll error");
        }
        if (m_pollFds[0].revents & POLLIN) {
            acceptConnection();
        }
    }
}
*/

void TcpServer2::acceptConnection() {
    int new_socket = accept(m_socket, (sockaddr *)&m_socketAddress, &m_socketAddress_len);
    if (new_socket < 0) {
        std::ostringstream ss;
        ss << "Server failed to accept incoming connection from ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: " << ntohs(m_socketAddress.sin_port);
        exitWithError(ss.str());
    }

    std::ostringstream ss;
    ss << "Accepted connection from address: " << inet_ntoa(m_socketAddress.sin_addr) << "; PORT: " << ntohs(m_socketAddress.sin_port);
    log(ss.str());

    //Buscar dados do pedido do clinte Metodo, path...
    std::string clientRequest = showClientHeader(new_socket);
    //Criar Resposta como os dados do Pedido
    Request request(clientRequest);
    Response response(m_server);
    request.verifyLocations(m_server);
    std::string serverResponse = response.buildResponse(request);
    //Enviar a resposta para o Cliente
    sendResponse(new_socket, serverResponse);

    close(new_socket);
}

std::string TcpServer2::showClientHeader(int client_socket) {
    char buffer[1024]; 
    int bytesReceived = recv(client_socket, buffer, sizeof(buffer), 0);
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
    std::cout << " * * * \n\n"<< std::endl;
}