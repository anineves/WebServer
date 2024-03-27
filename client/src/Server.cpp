#include "../includes/Server.hpp"

Server::Server()
{
}


//Acho que nao e necessario 
Server::Server(std::string ipAddr, int port, std::string root, std::string index) {

    s_ip_address = ipAddr;
    s_root = root;
    s_port = port;
    s_index = index;

    _upload_to = "";
    _methods.push_back("GET");
    _methods.push_back("POST");
    _methods.push_back("DELETE");
    _cgi_path = "";
    _cgi_ext = "";
    _executable = "";
    _redirect = false;
    _auto_index = false;
    s_socketAddress.sin_family = AF_INET;
    s_socketAddress.sin_port = htons(this->s_port);
    s_socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    if (setSocket()) {
        std::ostringstream ss;
        ss << "Failed to start server with Port: " << ntohs(s_socketAddress.sin_port);
        log(ss.str());
    }
    verificErrorServer();
}

Server::~Server() {
    std::cout << "Server port: " << this->s_port << " destructor called.\n";
}

// ---- SETTERS ----
void    Server::setIpAddr_s(std::string ipAddr) {this->s_ip_address = ipAddr;}

void    Server::setPort_s(int port) {this->s_port = port;}

void    Server::setRoot_s(std::string root) {this->s_root = root;}
void    Server::setIndex_s(std::string index) {this->s_index = index;}

void    Server::setErrorPage_s(std::string error_page) {this->s_error_page = error_page;}

void    Server::setLocation( std::vector<Location> locations) { this->_locations = locations; }

void    Server::setUploadTo(std::string upload_to) {this->_upload_to = upload_to;}

void Server::setMethods(std::vector<std::string> methods) {
    this->_methods.clear();
    this->_methods = methods;
}

void Server::setCgiPath(std::string cgi_path) { this->_cgi_path = cgi_path;}

void Server::setCgiExt(std::string cgi_ext) { this->_cgi_ext = cgi_ext;}

void Server::setAutoIndex(std::string autoindex) 
{ 
    if (autoindex == "true") {
        this->_auto_index = true;
    } else {
        this->_auto_index = false;
    }
}

void Server::setExecutable(std::string executable) 
{ 
    if (executable == "true") {
        this->_executable = true;
    } else {
        this->_executable = false;
    }
}

void Server::setRedirect(std::string redirect) 
{ 
    if (redirect == "true") {
        this->_redirect = true;
    } else {
        this->_redirect = false;
    }
}

int Server::setSocket() {

    this->s_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->s_socket < 0) {
        exitWithError("Cannot create socket.");
        return 1;
    }
    if (bind(this->s_socket, (sockaddr *)&s_socketAddress, s_socketAddress_len) < 0) {
        exitWithError("Cannot bind socket to address");
        return 1;        
    }
    return 0;
}


// ---- GETTERS ----
std::string Server::getIpAddr_s() {return this->s_ip_address;}

int     Server::getPort_s() {return this->s_port;}

std::string Server::getRoot_s() {return this->s_root;}

std::string Server::getIndex_s() {return this->s_index;}

std::string Server::getErrorPage_s() {return this->s_error_page;}

std::vector<Location> Server::getLocations() {return this->_locations;}

std::vector<std::string> Server::getMethods_s() {return this->_methods;}


void Server::verificErrorServer()
{
    if(s_ip_address.empty() || s_root.empty() || s_root.empty() || s_index.empty())
    {
        log("ERROR: Invalid Server");
            exit(1);
    }
}

void    Server::startListen() {
    if (listen(this->s_socket, 20) < 0) {
        exitWithError("Socket listen Failed.");
        exit(1);
    }

    std::ostringstream ss;
    ss << "\n*** Listening on ADDRESS: " << inet_ntoa(s_socketAddress.sin_addr) << " Port: " << ntohs(s_socketAddress.sin_port) << " ***\n\n";
    log(ss.str());
    this->e_poll_fd = epoll_create(MAXEPOLLSIZE);
    if (e_poll_fd < 0) {
        exitWithError("Epoll creat failed");
        exit(1);
    }
    else if (e_poll_fd) {
        log("Epoll Accepted!"); 
    }



}

