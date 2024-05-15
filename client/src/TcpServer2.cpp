#include "../includes/TcpServer2.hpp"

bool g_stop = 1;

void sighandler(int sig)
{
    if (sig == SIGINT || sig == SIGQUIT)
    {
        g_stop = 0;
        std::cout << " Thank you for your Time. " << std::endl;
    }
}

TcpServer2::TcpServer2(std::vector<Server> servers) : m_server(servers)
{
    setAddresses();
    startServer();
    startListen();
}

TcpServer2::~TcpServer2()
{
    closeServer();
    std::cout << "TcpServer Destructor called.\n";
}

void TcpServer2::startServer()
{
    std::vector<struct sockaddr_in>::iterator it;

    std::cout << "@#$@#@$##@$$# Tamanho addresses :" << m_addresses.size() << std::endl;
    std::cout << "\n\n * * * Listening Server at following ports * * *   \n\n";
    for (it = m_addresses.begin(); it != m_addresses.end(); ++it)
    {

        int curr_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (curr_socket < 0)
        {
            exitWithError("Socket creation failed");
            exit(EXIT_FAILURE);
        }
        int optval = 1;
        if (setsockopt(curr_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) != 0)
        {
            exitWithError("Not Reusing Socket");
            exit(EXIT_FAILURE);
        }
        if (bind(curr_socket, (struct sockaddr *)&*it, sizeof(*it)) < 0)
        {
            exitWithError("Bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(curr_socket, 20) < 0)
        {
            exitWithError("Socket listen Failed");
            exit(EXIT_FAILURE);
        }
        std::cout << "listening Port :" << BLUE << convert_uint32_to_str(ntohl(it->sin_addr.s_addr)) << ":" << ntohs(it->sin_port) << " | Socket: " << curr_socket << RESET << std::endl;
        this->m_sockets.push_back(curr_socket);
        // socketCreation[curr_socket] = time(NULL);
    }
}

void TcpServer2::startListen()
{
    this->epoll_fd = epoll_create(MAXEPOLLSIZE);
    if (epoll_fd == -1)
    {
        exitWithError("Create epoll");
        exit(EXIT_FAILURE);
    }

    struct epoll_event m_event;
    ft_memset(&m_event, 0, sizeof(m_event));

    struct epoll_event m_event_list[MAX_EVENTS];

    std::vector<int>::iterator it2;
    for (it2 = this->m_sockets.begin(); it2 != this->m_sockets.end(); it2++)
    {
        int add;
        m_event.events = EPOLLIN;
        m_event.data.fd = *it2;

        add = epoll_ctl(this->epoll_fd, EPOLL_CTL_ADD, *it2, &m_event);
        if (add == -1)
        {
            exitWithError("failed to add to epoll");
            exit(EXIT_FAILURE);
        }
    }

    signal(SIGINT, sighandler);
    signal(SIGQUIT, sighandler);

    while (g_stop == 1)
    {
        std::cout << "Stop antes " << g_stop << std::endl;
        int num_events = epoll_wait(epoll_fd, m_event_list, MAXEPOLLSIZE, 2000);
        if (num_events == -1)
        {
            std::cout << "Stop" << g_stop << std::endl;
            exitWithError("Epoll wait");
            continue;
        }
        verificTimeOut();

        std::cout << "num_events= " << num_events << std::endl;

        std::cout << "\n\n";
        for (int i = 0; i < num_events; i++)
        {
            std::cout << m_event_list[i].data.fd << std::endl;
        }
        std::cout << "\n\n";
        for (int i = 0; i < num_events; i++)
        {
            for (size_t j = 0; j < this->m_server.size(); ++j)
            {
                if (m_event_list[i].data.fd == m_sockets[j])
                {
                    acceptNewConnection(m_event_list[i], m_event_list[i].data.fd, j);
                }
                else if (m_event_list[i].events & EPOLLIN)
                {
                    handleInput(m_event_list[i], m_event_list[i].data.fd);
                }
                else if (m_event_list[i].events & EPOLLOUT)
                {
                    handleOutput(m_event_list[i], m_event_list[i].data.fd);
                }
            }
        }
    }
    closeConnection();
}



void TcpServer2::acceptNewConnection(epoll_event &m_event, int fd, int j)
{
    m_server[j].setSocketAddr_len(sizeof(m_sockets[j]));
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int client_socket = accept(fd, (struct sockaddr *)&addr, &addr_len);
    std::cout << CYAN << client_socket << RESET << std::endl;
    std::cout << "entrei accept " << std::endl;
    if (client_socket == -1)
    {
        exitWithError("Can't Accept something");
        return;
    }
    std::cout << "New Connection on port: " << ntohs(m_server[j].sin_port) << "| fd:" << client_socket << std::endl;

    m_event.events = EPOLLIN | EPOLLRDHUP;
    m_event.data.fd = client_socket;
    if (epoll_ctl(this->getEpoll(), EPOLL_CTL_ADD, client_socket, &m_event) == -1)
    {
        exitWithError("Problems in Epoll_CTL.");
        exit(EXIT_FAILURE);
    }
    // Associar cada socket a cada client
    clientServerMap[client_socket] = &m_server[j];
    socketCreation[client_socket] = time(NULL); // Atualiza o tempo de criação do socket
}



void TcpServer2::handleInput(epoll_event &m_event, int fd)
{
    Server *server = clientServerMap[fd];
    if (server != NULL)
    {
        socketCreation[fd] = time(NULL); // Atualiza o tempo de criação do socket
        Request request1;

        showClientHeader(m_event, request1);
        if (!request1.has_header)
        {
            return;
        }
        // Request request(clientRequest);
        request1.verific_errors(*server);
        std::string serverResponse;
        Response response(*server);
        std::cout << CYAN << "Path from request = " << request1.getPath() << " CODE " << request1.getCode() << RESET << std::endl;
        if (request1.getCode() != 200 && !request1.getPath().empty())
        {
            serverResponse = response.buildErrorResponse(request1.getCode());
            m_event.events = EPOLLOUT;
            epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, fd, &m_event);
            responseMap[fd] = serverResponse;
        }
        else
        {
            Location locationSettings = server->verifyLocations(request1.getPath());
            int n = 0;
            if (is_file("frontend/html" + request1.getPath()) == 1)
            {
                n = 1;
            }
            if (!locationSettings.getPath().empty())
            {
                std::cout << locationSettings.getPath() << " " << locationSettings.getAllowMethods()[0] << std::endl;
                if (locationSettings.getAllowMethods()[0] == "DELETE")
                {
                    std::string pathToDelete = "frontend2" + request1.getPath();
                    if (std::remove(pathToDelete.c_str()) != 0)
                        serverResponse = response.buildErrorResponse(500);
                    else
                        serverResponse = "HTTP/1.1 200 OK\r\n";
                }
                else if (!locationSettings.getReturn().empty())
                {
                    std::istringstream iss(locationSettings.getReturn());
                    std::string response;
                    std::string code;
                    std::string loc;
                    iss >> code;
                    iss >> loc;
                    response = "HTTP/1.1 " + code + " Moved Permanently \r\n";
                    response += "Content-Length: 0\r\n";
                    response += "Location: " + loc + "\r\n\r\n";
                    serverResponse = response;
                }
                else if (!locationSettings.getCgiPath().empty())
                {
                    if (!request1.getPath().empty()) {
                        Cgi cgi(request1.getPath());
                        serverResponse = cgi.runCgi(request1); // Armazena a resposta do CGI
                    }
                }
                else if (locationSettings.getAutoIndex() == "on" && n == 0)
                {
                    DIR *dir;
                    struct dirent *ent;
                    std::vector<std::string> content;
                    std::cout << "locationsettings = " << locationSettings.getPath() << std::endl;
                    if ((dir = opendir(("frontend/html" + locationSettings.getPath() + "/").c_str())) != NULL)
                    {
                        while ((ent = readdir(dir)) != NULL)
                        {
                            if (ent->d_type == DT_REG)
                            {
                                std::string file_name = ent->d_name;
                                content.push_back(file_name);
                            }
                            else if (ent->d_type == DT_DIR)
                            {
                                std::string dir_name = ent->d_name;

                                if (dir_name != "." && dir_name != "..")
                                    content.push_back(dir_name + "/");
                            }
                        }
                        closedir(dir);
                    }
                    serverResponse = dirListHtml(content);
                }
                else
                {
                    serverResponse = response.buildResponse(request1);
                }
                m_event.events = EPOLLOUT;
                epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, fd, &m_event);
                responseMap[fd] = serverResponse;
            }
        }
    }
}

void TcpServer2::handleOutput(epoll_event &event, int fd)
{
    std::string serverResponse = responseMap[fd];
    sendResponse(fd, serverResponse);
    responseMap.erase(fd);
    event.events = EPOLLIN;
    epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, fd, &event);
    int ret = epoll_ctl(this->getEpoll(), EPOLL_CTL_DEL, fd, &event);
    if (ret == -1)
    {
        std::cout << "failed to remove fd " << fd << " from EPOLL" << std::endl;
    }
    close(fd);
}

size_t stringtohex(std::string value)
{
    std::istringstream iss(value);
    size_t int_value;

    iss >> std::hex >> int_value;
    return int_value;
}

void TcpServer2::showClientHeader(struct epoll_event &m_events, Request &request)
{

    char buffer[5000];
    int bytesReceived;
    bool chunked = false;
    bool first = true;

    std::string chunk;
    std::string content_length;
    std::string chunk_length_str;

    do
    {
        ft_memset(&buffer, 0, 5000);
        bytesReceived = recv(m_events.data.fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
        if (bytesReceived < 0)
            break ;         
        if(bytesReceived == 0) 
            close(m_events.data.fd);
        this->_client_request.append(buffer, bytesReceived);

        size_t found_header = this->_client_request.find("\r\n\r\n");
        if (found_header != std::string::npos && this->_header.empty()) {
            request.has_header = true;
            this->_header = this->_client_request.substr(0, found_header + 4);
            if (this->_header.find("Content-Length") != std::string::npos) {
                size_t pos = this->_header.find("Content-Length:");
                pos += 15;
                size_t end_pos = this->_header.find("\r\n", pos);
                content_length = this->_header.substr(pos, end_pos - pos);
            }
            else if (this->_header.find("Transfer-Encoding") != std::string::npos)
                chunked = true;
        }
        if (chunked == true) {
            if (first == true) {
                this->_body = this->_client_request.substr(found_header + 4, this->_client_request.size());
                this->_client_request.clear();
                first = false;
            }
            else {
                this->_body.append(this->_client_request);
                this->_client_request.clear();
            }
            chunk_length_str = this->_body.substr(0, this->_body.find("\r\n"));
            if (this->_body.size() >= stringtohex(chunk_length_str)) {
                while (this->_body.size() > stringtohex(chunk_length_str)) {
                    chunk += this->_body.substr(this->_body.find("\r\n") + 2, stringtohex(chunk_length_str));
                    this->_body.erase(0, stringtohex(chunk_length_str) + 2 + chunk_length_str.size() + 2);                
                    chunk_length_str = this->_body.substr(0, this->_body.find("\r\n"));
                }
            }
            chunk_length_str.clear();
        }
    } while (bytesReceived > 0);

    if(request.has_header == true) {
        if (chunked) {
            this->_header += chunk;
            request.parser(this->_header);
            request._fullRequest += this->_header;
        }
        else {
            request.parser(this->_client_request);
            request._fullRequest += this->_client_request;
        }
        this->_header.clear();
        this->_body.clear();
        this->_client_request.clear();
    }
}

void TcpServer2::sendResponse(int client_socket, const std::string &response)
{
    long bytesSent = send(client_socket, response.c_str(), response.size(), 0);
    if (bytesSent == (long)response.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else
    {
        log("Error sending response to client");
    }
}

void TcpServer2::closeServer()
{
    std::cout << "closeServer Function called.\n";
    exit(0);
}

uint32_t TcpServer2::strToNet(const std::string &ip_address)
{
    struct in_addr addr;
    if (inet_pton(AF_INET, ip_address.c_str(), &addr) <= 0)
    {
        std::cerr << "Error converting IP address" << std::endl;
        return 0;
    }
    return htonl(addr.s_addr);
}

int TcpServer2::getEpoll()
{
    return this->epoll_fd;
}

void TcpServer2::setAddresses()
{

    std::vector<Server>::iterator it;
    for (it = this->m_server.begin(); it != this->m_server.end(); ++it)
    {
        struct sockaddr_in curr;

        ft_memset(&(curr), 0, sizeof(curr));
        curr.sin_family = AF_INET;
        curr.sin_port = it->sin_port;
        curr.sin_addr.s_addr = it->s_addr;

        std::vector<struct sockaddr_in>::iterator it2;
        for (it2 = m_addresses.begin(); it2 != m_addresses.end(); it2++)
        {
            if (it2->sin_addr.s_addr == curr.sin_addr.s_addr && it2->sin_port == curr.sin_port)
                break;
        }
        if (it2 == m_addresses.end())
            m_addresses.push_back(curr);
    }
}

void TcpServer2::verificTimeOut()
{
    time_t currentTime = time(NULL);
    for (std::map<int, time_t>::iterator it = socketCreation.begin(); it != socketCreation.end();)
    {
        time_t elapsedTime = currentTime - it->second;
        std::cout << YELLOW << it->first << " elapsedTime = " << elapsedTime << RESET << std::endl;
        if (elapsedTime >= TIMEOUT)
        {
            std::cout << MAGENTA << "Vou fechar conexão :" << it->first << RESET << std::endl;
            close(it->first);
            epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, it->first, NULL);
            clientServerMap.erase(it->first);
            responseMap.erase(it->first);
            socketCreation.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

void TcpServer2::closeConnection()
{
    std::cout << "Vou eliminar tudo" << std::endl;
    for (std::vector<int>::iterator it = m_sockets.begin(); it != m_sockets.end();)
    {
        close(*it);
        it++;
        epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, *it, NULL);
    }

    for (std::map<int, time_t>::iterator it = socketCreation.begin(); it != socketCreation.end();)
    {
        close(it->first);
        epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, it->first, NULL);
        clientServerMap.erase(it->first);
        responseMap.erase(it->first);
        socketCreation.erase(it++);
    }
    m_sockets.clear();
    m_addresses.clear();
    // m_sockets.erase();

    exit(EXIT_SUCCESS);
}
