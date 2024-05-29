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

TcpServer2::TcpServer2(std::vector<Server> &servers) : m_server(servers)
{
    timeout = false;
    _fullheader = false;
    _header = "";
    _max_length = 0;
    _chunked = false;
    _first_chunk = true;
    _my_bytes = 0;
    chunk = "";
    chunk_length_str = "";
    _last_chunk = false;
    _body = "";
    setAddresses();
    startServer();
    startListen();
}

TcpServer2::~TcpServer2()
{
    std::cout << "TcpServer Destructor called" << std::endl;
    closeConnection();
}
 /*Cria e configura sockets para os servidores definidos.
Liga os sockets aos endereços especificados.
Inicia a escuta nos sockets. */
void TcpServer2::startServer()
{
    std::vector<struct sockaddr_in>::iterator it;

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
    }
}

/*Cria um epoll para monitorar os sockets.
Adiciona os sockets ao epoll.
Entra em um loop para esperar por eventos nos sockets.
Quando um evento ocorre, trata o evento de acordo com o tipo (entrada, saída, etc.).
*/
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
        // epoll_wait(epoll_fd, events, MAX_EVENTS, TIMEOUT);
        int num_events = epoll_wait(epoll_fd, m_event_list, MAXEPOLLSIZE, 2000);
        if (num_events == -1)
        {
            exitWithError("Epoll wait");
            continue;
        }

        verificTimeOut();
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

    return;
}

/*Aceita uma nova conexão de cliente.
Adiciona o novo socket ao epoll para monitoramento.*/
void TcpServer2::acceptNewConnection(epoll_event &m_event, int fd, int j)
{
    m_server[j].setSocketAddr_len(sizeof(m_sockets[j]));
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int client_socket = accept(fd, (struct sockaddr *)&addr, &addr_len);

    if (client_socket == -1)
    {
        exitWithError("Can't Accept something");
        return;
    }
    std::cout << "New Connection on port: " << ntohs(m_server[j].sin_port) << std::endl;

    m_event.events = EPOLLIN | EPOLLRDHUP;
    m_event.data.fd = client_socket;
    if (epoll_ctl(this->getEpoll(), EPOLL_CTL_ADD, client_socket, &m_event) == -1)
    {
        exitWithError("Problems in Epoll_CTL.");
        exit(EXIT_FAILURE);
    }
    // Associar cada socket a cada client
    clientServerMap[client_socket] = &m_server[j];
    socketCreation[client_socket] = time(NULL);
}


/*Lida com a entrada de dados dos clientes.
Processa as requisições HTTP recebidas.
Decide como responder com base na requisição recebida.
*/
void TcpServer2::handleInput(epoll_event &m_event, int fd)
{
    Server *server = clientServerMap[fd];
    if (server != NULL)
    {
        Request request1;
        showClientHeader(m_event, request1);
        if (!_fullheader)
            return;
        Response response(*server);
        std::string serverResponse;
        if (timeout == true)
        {
            serverResponse = response.buildErrorResponse(request1.getCode());
            m_event.events = EPOLLOUT;
            epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, fd, &m_event);
            responseMap[fd] = serverResponse;
            timeout = false;
            return;
        }
        socketCreation[fd] = time(NULL);

        request1.verific_errors(*server, this->_max_length);
        if(request1.getCode() == 400)
        {
            close(fd);
            return;
        }
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
            locationSettings.deleteAllowed = false;
           

            bool not_allow = 0;
            if (!locationSettings.getPath().empty())
            {
                 if (locationSettings.getRoot() == "")
                    locationSettings.setRoot(server->getRoot_s());

                int n = 0;
                if (is_file(locationSettings.getRoot() + "/html" + request1.getPath()) == 1 || is_file(locationSettings.getRoot() + "/" + request1.getPath()) == 1)
                {
                    n = 1;
                }
                int is_dir = 0;
                if(is_directory(locationSettings.getRoot() + "/" + request1.getPath()) == 1)
                {
                    is_dir = 1;
                }

                for (size_t i = 0; i < locationSettings.getAllowMethods().size(); i++)
                {
                    if (locationSettings.getAllowMethods()[i] == request1.getMethod())
                        not_allow = 1;
                    if (locationSettings.getAllowMethods()[i] ==  "DELETE")
                        locationSettings.deleteAllowed = true;
                
                }
                if (!locationSettings.getReturn().empty())
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
                else if(is_dir == 1 && locationSettings.getAutoIndex() != "on" && locationSettings.getPath() != "/")
                {
                    serverResponse = response.buildErrorResponse(404);
                }
                else if (not_allow == 0)
                {
                    serverResponse = response.buildErrorResponse(405);
                }
                else if (!locationSettings.getCgiPath().empty())
                {
                    if (!request1.getPath().empty())
                    {
                        Cgi cgi(request1.getPath());
                        serverResponse = cgi.runCgi(request1);
                        if(request1.getCode() != 200)
                            serverResponse = response.buildErrorResponse(500);
                    }
                }
                else if (locationSettings.getAutoIndex() == "on" && n == 0)
                {
                    DIR *dir;
                    struct dirent *ent;
                    std::vector<std::string> content;
                    if ((dir = opendir((locationSettings.getRoot() + "/" + locationSettings.getPath() + "/").c_str())) != NULL)
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
                        serverResponse = dirListHtml(content);
                    }
                    else
                    {
                        serverResponse = response.buildErrorResponse(404);
                    }
                }
                else if (locationSettings.deleteAllowed && request1.getMethod() == "DELETE")
                {
                    std::string pathToDelete = server->getRoot_s() + request1.getPath();
                    if(!fileExists(pathToDelete))
                        serverResponse = response.buildErrorResponse(404);
                    else if (std::remove(pathToDelete.c_str()) != 0)
                        serverResponse = response.buildErrorResponse(500);
                    else
                        serverResponse = response.buildResponseDelete(request1, locationSettings);
                }
                else
                {
                    serverResponse = response.buildResponse(request1, locationSettings);
                }
                m_event.events = EPOLLOUT;
                epoll_ctl(this->getEpoll(), EPOLL_CTL_MOD, fd, &m_event);
                responseMap[fd] = serverResponse;
            }
        }
        _fullheader = false;
        this->_max_length = 0;
    }
}

/*Lida com a saída de dados para os clientes.
Envia as respostas HTTP para os clientes.*/
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
    size_t int_value = 0;

    iss >> std::hex >> int_value;
    return int_value;
}

void    TcpServer2::fetchAndParseRequest(Request &request)
{
    if (_chunked) {
        if (_last_chunk == true) {
            this->_header.append(chunk);
            request.parser(this->_header);
            request._fullRequest += this->_header;
            _fullheader = true;
            _chunked = false; 
            _last_chunk = false;
            _first_chunk = true;
            chunk.clear();
            this->_header.clear();
            this->_body.clear();
            this->_client_request.clear();
        }
    }
    else if (this->_max_length <= _my_bytes || this->_header.find("POST") == std::string::npos) {
        usleep(1000);
        _fullheader = true;
        request.parser(this->_client_request);
        request._fullRequest += this->_client_request;
        this->_header.clear();
        this->_body.clear();
        this->_client_request.clear();
        _my_bytes = 0;
        this->_has_header = false;
    }
}

int    TcpServer2::handleChunkedRequest(size_t found_header, Request &request)
{
    /*if (_header.find("GET") != std::string::npos) {
        std::cout << "entrei " << std::endl;
        _fullheader = true;
        request.chunked_error = true;
        _client_request.clear();
        this->_header.clear();
        return 1;
    }*/
    if (_first_chunk == true) {
        _body = _client_request.substr(found_header + 4, _client_request.size());
        std::cout << GREEN << "_body:\n" << _body << RESET << std::endl;
        if (_header.find("POST") != std::string::npos && _body.empty()) {
            if (_client_request.find("100-continue") == std::string::npos) {
                _fullheader = true;
                request.chunked_error = true;
                _client_request.clear();
                this->_header.clear();
                return 1;
            }
        }
        _first_chunk = false;
    }
    else
        _body.append(_client_request);
    chunk_length_str = _body.substr(0, _body.find("\r\n"));
    _chunk_size = stringtohex(chunk_length_str);
    if (_body.size() >= _chunk_size) 
    {
        while (_body.size() > stringtohex(chunk_length_str)) 
        {
            _chunk_size = stringtohex(chunk_length_str);
            if (_chunk_size == 0)
                _last_chunk = true;
            _max_length += _chunk_size;
            chunk += _body.substr(_body.find("\r\n") + 2, _chunk_size);
            _body.erase(0, _chunk_size + 2 + chunk_length_str.size() + 2);
            chunk_length_str = _body.substr(0, _body.find("\r\n"));
        }
        chunk_length_str.clear();
    }
    _client_request.clear();
    return 0;
}

/*Lê o cabeçalho da requisição HTTP enviada pelos clientes.
Extrai informações importantes do cabeçalho, como método, URI, tamanho do conteúdo, etc.*/
void TcpServer2::showClientHeader(struct epoll_event &m_events, Request &request)
{
    char buffer[5000];
    int bytesReceived = 0;
    
    memset(&buffer, 0, sizeof(buffer));
    bytesReceived = recv(m_events.data.fd, buffer, sizeof(buffer) - 1, MSG_NOSIGNAL);
    _my_bytes += bytesReceived;
    if (bytesReceived <= 0) {
        close(m_events.data.fd);
        return;
    }
    _client_request.append(buffer, bytesReceived);
    size_t found_header = _client_request.find("\r\n\r\n");
    if (found_header != std::string::npos && _header.empty())
    {
        _has_header = true;
        _header = _client_request.substr(0, found_header + 4);
        size_t content_pos = _header.find("Content-Length:");
        if (content_pos != std::string::npos) {
            content_pos += 15;
            size_t end_pos = _header.find("\r\n", content_pos);
            _max_length = std::atoi(_header.substr(content_pos, end_pos - content_pos).c_str());
        }
        else if (_header.find("Transfer-Encoding") != std::string::npos)
            _chunked = true;
        else if (_header.find("POST") != std::string::npos)
            request.no_length = true;
        if (content_pos != std::string::npos && _header.find("Transfer-Encoding") != std::string::npos) {
            request.chunked_error = true;
        }
    }
    if (_chunked == true)
        if (handleChunkedRequest(found_header, request))
            return ;
    if (_has_header)
        fetchAndParseRequest(request);
    request.printMap();
}

void TcpServer2::sendResponse(int client_socket, const std::string &response)
{
    //std::cout << MAGENTA << response << RESET << std::endl;
    long bytesSent = send(client_socket, response.c_str(), response.size(), 0);
    if (bytesSent == (long)response.size())
    {
        log("------ Server Response sent to client ------\n\n");
    }
    else if (bytesSent <= 0)
    {
        log("Error sending response to client");
        close(client_socket);
    }
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
        if (elapsedTime >= TIMEOUT)
        {
            std::cout << "Close conection: " << MAGENTA << it->first << RESET << std::endl;
            epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, it->first, NULL);
            close(it->first);
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
    for (std::vector<int>::iterator it = m_sockets.begin(); it != m_sockets.end(); ++it)
    {
        epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, *it, NULL);
        close(*it);
    }
    m_sockets.clear();

    for (std::map<int, time_t>::iterator it = socketCreation.begin(); it != socketCreation.end();)
    {
        epoll_ctl(this->epoll_fd, EPOLL_CTL_DEL, it->first, NULL);
        close(it->first);
        clientServerMap.erase(it->first);
        responseMap.erase(it->first);
        socketCreation.erase(it++);
    }

    for (size_t i = 0; i < m_server.size(); ++i)
    {
        m_server[i]._locations.clear();
    }

    m_server.clear();
    m_addresses.clear();
    m_sockets.clear();
    _header.clear();
    _client_request.clear();
    m_server.clear();

    std::string().swap(_header);
    std::string().swap(_client_request);
    std::vector<int>().swap(m_sockets);
    std::vector<Server>().swap(m_server);
}

