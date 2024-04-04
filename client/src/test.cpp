#include "../includes/TcpServer2.hpp"

TcpServer2::TcpServer2(std::vector<Server> servers) : m_server(servers) {
    // Inicializa e inicia todos os servidores em m_server
     for (size_t i = 0; i < m_server.size(); ++i) {
        initializeServer(m_server[i]);
        startServer(m_server[i]);
    }
}

TcpServer2::~TcpServer2() {
    this->closeServer();
    std::cout << "TcpServer Destructor called.\n";
}


void TcpServer2::initializeServer(Server& server) {
    // Criação do socket
    int curr_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (curr_socket < 0) {
        exitWithError("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configuração para reutilizar endereços, se necessário
    int optval = 1;
    if (setsockopt(curr_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0) {
        exitWithError("Not Reusing Socket");
        exit(EXIT_FAILURE);
    }

    // Configuração do endereço do servidor
    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(server.getPort_s());

    // Associação do socket ao endereço
    if (bind(curr_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        exitWithError("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Início da escuta
    if (listen(curr_socket, 20) < 0) {
        exitWithError("Socket listen Failed");
        exit(EXIT_FAILURE);
    }

    // Adição do socket ao epoll para monitoramento de eventos
    struct epoll_event event;
    event.events = EPOLLIN;
    event.data.fd = curr_socket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, curr_socket, &event) == -1) {
        exitWithError("failed to add to epoll");
        exit(EXIT_FAILURE);
    }

    // Atualiza as informações relevantes no objeto Server
    server.setSocket(curr_socket);
    server.setSocketAddr(server_address);
}

void TcpServer2::startServer(Server& server) {
    // Inicialização da lista de eventos
    struct epoll_event m_event_list[MAX_EVENTS];
    memset(m_event_list, 0, sizeof(m_event_list));

    // Loop principal para lidar com eventos
    while (1) {
        // Recebe eventos usando epoll
        int num_events = epoll_wait(epoll_fd, m_event_list, MAXEPOLLSIZE, 2000);
        if (num_events == -1) {
            exitWithError("Epoll wait");
            continue;
        }
        for (int i = 0; i < num_events; i++) {
            // Lida com eventos para o servidor especificado
            if (m_event_list[i].data.fd == server.getSocket()) {
                handleNewConnection(server, m_event_list[i]);
            } else if (m_event_list[i].events & EPOLLIN) {
                std::string clientRequest = showClientHeader(m_event_list[i]);
                    std::cout << "########### get " << m_server[j].getExecutable() << std::endl;
                    Request request(clientRequest);
                    std::cout << "@@@@@@@@@@@@ TCP Location" << m_server[j].getLocations()[1].getCgiExt() << std::endl;
                    request.verifyLocations(m_server[j]);
                    Response response(m_server[j]);
                    std::string serverResponse = response.buildResponse(request);
                    m_event_list[i].events = EPOLLOUT;
                    epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, m_event_list[i].data.fd, &m_event_list[i]);
                    responseMap[m_event_list[i].data.fd] = serverResponse;
            } else if (m_event_list[i].events & EPOLLOUT) {
                 std::string serverResponse = responseMap[m_event_list[i].data.fd];
                    sendResponse(m_event_list[i].data.fd, serverResponse);
                    responseMap.erase(m_event_list[i].data.fd);
                    m_event_list[i].events = EPOLLIN;
                    epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, m_event_list[i].data.fd, &m_event_list[i]);
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
    for (size_t i = 0; i < m_server.size(); i++) {

        m_addresses.push_back(m_server[i].getSocketAddr());

    }
    std::cout << "m_adresses size = " << m_addresses.size() << std::endl;
}
