#include "../includes/Server.hpp"

Server::Server() {
    
    memset(&s_socketAddress, 0, sizeof(s_socketAddress));
    s_socketAddress.sin_family = AF_INET;
    s_socketAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    std::cout << "valor Porta" << htons(this->s_port) << std::endl;
    s_socketAddress.sin_port = htons(this->s_port);
}

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
    verificErrorServer();
}

Server::~Server() {
    close(this->s_socket);
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

void Server::setSocket(int set_socket) {
    this->s_socket = set_socket;
}

void    Server::setSocketAddr(struct sockaddr_in n_socketaddr) {
    this->s_socketAddress = n_socketaddr;
}

void    Server::setSocketAddr_len(socklen_t n_socketaddr_len) {
    this->s_socketAddress_len = n_socketaddr_len;
}

// ---- END GETTERS -----



// ---- GETTERS ----

std::string Server::getIpAddr_s() {
    return this->s_ip_address;
}

int     Server::getPort_s() {
    return this->s_port;
}

std::string Server::getRoot_s() {
    return this->s_root;
}

std::string Server::getIndex_s() {
    return this->s_index;
}

std::string Server::getErrorPage_s() {
    return this->s_error_page;
}

std::vector<Location> Server::getLocations() {
    return this->_locations;
}

std::vector<std::string> Server::getMethods_s() {
    return this->_methods;
}

int Server::getSocket() {
    return this->s_socket;
}

struct sockaddr_in Server::getSocketAddr() {
    return this->s_socketAddress;
}

socklen_t  Server::getSocketAddr_len() {
    return this->s_socketAddress_len;
}   

// ---- END GETTERS -----
void Server::verificErrorServer()
{
    if(s_ip_address.empty() || s_root.empty() || s_root.empty() || s_index.empty())
    {
        log("ERROR: Invalid Server");
            exit(1);
    }
}