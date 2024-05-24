#include "../includes/TcpServer2.hpp"
#include "../includes/ConfigFile.hpp"
#include "../includes/cParser.hpp"

int validFile(std::string arg) {
    size_t dotPosition = arg.find_last_of('.');
    std::string extension = arg.substr(dotPosition);
    if (extension != ".conf")
        return 0;
    return 1;
}

int main(int ac, char **av) {
    std::string confFile;
    if (ac <= 2) {
        if (ac == 1)
            confFile = "./config/Default.conf";
        else if (!validFile(av[1]) && av[1]) {
            std::cout << "Error: File extension not valid.\nUsing Default file\n";
            confFile = "./config/Default.conf";
        } else if (validFile(av[1]) && av[1]) {
            std::ifstream unexistingFile(av[1]);
            if (!unexistingFile.is_open()) {
                confFile = "./config/Default.conf";
            } else {
                confFile = "./" + std::string(av[1]);
            }
        }
    } else {
        std::cout << "Error: Invalid args!\n";
        return 0;
    }
    try {
        cParser myFile;
        myFile.getFileVec(confFile);
        //std::cout << GREEN << "This file is validated!\n" << RESET;
    } catch (const char* errorMessage) {
        std::cout  << BOLD << RED << "Error: " << RESET << errorMessage << std::endl;
        return 0;
    }
    ConfigFile configFile(confFile);
    std::string ip = configFile.getIpAddr();
    std::vector<Server>& servers = configFile.getServers();

    TcpServer2 tcpServer(servers);

    return 0;
}
