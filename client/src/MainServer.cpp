#include "../includes/TcpServer2.hpp"
#include "../includes/ConfigFile.hpp"
#include "../includes/Server.hpp"



int validFile(std::string arg) {
    size_t dotPosition = arg.find_last_of('.');
    std::string extension = arg.substr(dotPosition);
    if (extension != ".conf")
        return 0;
    return 1;
}

int main(int ac, char **av) {
    std::string confFile;
    if (ac <= 2 ) {
        if (ac == 1)
            confFile = "./config/Default.conf";
        else if (!validFile(av[1]) && av[1]) 
        {
            std::cout << "Error: File extension not valid.\nUsing Default file\n";
            confFile = "./config/Default.conf";
        }
        else if (validFile(av[1]) && av[1]) {
            std::ifstream unexistingFile(av[1]);
            if (!unexistingFile.is_open()) {
                confFile = "./config/Default.conf";
            }
            confFile = "./" + std::string(av[1]);
        }
    else
    {
        std::cout << "Error: Invalid args!\n";
        return 0;
    }
    ConfigFile configFile(confFile);
    std::string ip = configFile.getIpAddr();
    std::cout << "ip ======== " << ip << std::endl;

    std::vector<Server>& servers = configFile.getServers();
     //std::vector<struct sockaddr_in> unique_addrs = configFile.get_unique_addresses();

    
    TcpServer2 tcpServer(servers);
    //tcpServer.startServer(unique_addrs);

    //tcpServer.startListen();
    return 0;
}
}
//./webserv config/DefaultConfig.conf can work because the file doesnt exist, and should use default or error dunno :/

// Fazes class server, que tem um vetor, onde garda a informacao de cada server. 
// Ver o location o que e e como funciona. 
// Ver respostas
// Ver o CGI
// Esta sempre a enviar o index.html,perceber como enviar o css e o register