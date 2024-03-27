#include "../includes/TcpServer2.hpp"

TcpServer2::TcpServer2(Server& server)
    : m_socket(-1), m_socketAddress(), m_socketAddress_len(sizeof(m_socketAddress)), m_server(server) {
    m_socketAddress.sin_family = AF_INET;
    m_socketAddress.sin_port = htons(server.getPort_s());
    m_socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);

    if (this->startServer() != 0) {
        std::ostringstream ss;
        ss << "Failed to start server with Port: " << ntohs(m_socketAddress.sin_port);
        log(ss.str());
    }
}

TcpServer2::~TcpServer2() {
    this->closeServer();
    std::cout << "TcpServer Destructor called.\n";
}

int TcpServer2::startServer() {
/*     m_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_socket < 0) {
        exitWithError("Cannot create socket");
        return 1;
    }
    if (bind(m_socket, (sockaddr *)&m_socketAddress, m_socketAddress_len) < 0) {
        exitWithError("Cannot bind socket to address");
        return 1;
    } */
    return 0;
}

void TcpServer2::startListen() {
    //correr os servers e o Epoll eu todos ate ficarem ready (POLLIN || POLLOUT)
    if (listen(m_socket, 20) < 0) {
        exitWithError("Socket listen Failed");
    }

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(m_socketAddress.sin_addr) << " Port: " << ntohs(m_socketAddress.sin_port) << " ***\n\n";
    log(ss.str());

    m_pollFds[0].fd = m_socket;
    m_pollFds[0].events = POLLIN;

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
    close(m_socket);
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

