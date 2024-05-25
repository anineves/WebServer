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
    socklen_t ad