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
void    Server::setUploadTo(std::string upload_to) {this->_upload_to = upload_to;}
void    Server::setLocation( std::vector<Location> locations) { this->_locations = locations; }

// ---- GETTERS ----
std::string Server::getIpAddr_s() {return this->s_ip_address;}

int     Server::getPort_s() {return this->s_port;}

std::string Server::getRoot_s() {return this->s_root;}

std::string Server::getIndex_s() {return this->s_index;}
std::string Server::getErrorPage_s() {return this->s_error_page;}
std::vector<Location> Server::getLocations() {return this->_locations;}



void Server::verificErrorServer()
{
    if(s_ip_address.empty() || s_root.empty() || s_root.empty() || s_index.empty())
    {
        log("ERROR: Invalid Server");
                exit(1);
    }
}



