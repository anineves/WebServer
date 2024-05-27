#include "../includes/Server.hpp"

Server::Server()
    : s_root(""),
      s_index(""),
      s_index_pages(),
      s_error_page(""),
      s_ip_address(""),
      s_port(""),
      s_client_body(0),
      _upload_to(""),
      _cgi_path(""),
      _cgi_ext(""),
      _auto_index(false),
      _executable(false),
      _return(""),
      s_socket(0),
      _redirect(false),
      s_socketAddress_len(0),
      _locations(),
      s_addr(0),
      sin_port(0),
      s_host(""),
      s_server_names() {
}


Server::Server(const Server &source)
{
    *this= source;
    return ;
}

 Server &Server::operator=(const Server &rhs) {
            if (this != &rhs) {
                s_ip_address = rhs.s_ip_address;
                s_root = rhs.s_root;
                s_port = rhs.s_port;
                s_index = rhs.s_index;
                s_error_page = rhs.s_error_page;
                s_client_body = rhs.s_client_body;
            

                _upload_to = rhs._upload_to;
                _methods = rhs._methods;
                _cgi_path = rhs._cgi_path;
                _cgi_ext = rhs._cgi_ext;
                _executable = rhs._executable;
                _redirect = rhs._redirect;
                _auto_index = rhs._auto_index;

                _return = rhs._return;
                s_socket = rhs.s_socket;
                s_socketAddress_len = rhs.s_socketAddress_len;
                if (!rhs._locations.empty()) 
                {
                    _locations.clear();
                    _locations = rhs._locations;
                }
                if (!rhs.s_server_names.empty()) 
                {
                    s_server_names.clear();
                    s_server_names = rhs.s_server_names;
                }
                s_addr = rhs.s_addr;
                sin_port = rhs.sin_port;
                s_host = rhs.s_host;
                s_socketAddress = rhs.s_socketAddress; 
        }
            return *this;
 }



Server::~Server() {
    //close(this->s_socket);
    //std::cout << "Server port: " << this->s_port << " destructor called.\n";
    //_locations.clear();
    _methods.clear();
    s_server_names.clear();
    s_index_pages.clear();

}

// ---- SETTERS ----
void    Server::setIpAddr_s(std::string ipAddr) {this->s_ip_address = ipAddr;}

void    Server::setPort_s(std::string port) {this->s_port = port;}

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


void Server::setClientMaxBody(size_t client_max_body) {
    this->s_client_body = client_max_body;
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

std::string     Server::getPort_s() {
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

size_t Server::getClientMaxBody_s() {
    return this->s_client_body;
}

struct sockaddr_in Server::getSocketAddr() {
    return this->s_socketAddress;
}

socklen_t  Server::getSocketAddr_len() {
    return this->s_socketAddress_len;
}   

bool Server::getExecutable()
{
    return this->_executable;
}

bool Server::getRedirect()
{
    return this->_redirect;
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


Location Server::verifyLocations(std::string pathRequest) {
    std::string bestMatchPath;
    Location location;

    int location_found = 0;
    size_t extensionPos;

    while (!location_found) {
        for (size_t i = 0; i < _locations.size(); i++) {

            size_t extensionPosDot = pathRequest.find_last_of('.');
            if (extensionPosDot != std::string::npos) 
            {
                    std::string pathExt = pathRequest.substr(extensionPosDot);
                    if(pathExt == ".py")
                    {
                        pathRequest = pathExt;
                       if(_locations[i].getPath().find_last_of('.'))
                       {
                            location = _locations[i];
                       }
                    }
                    
            }
            if (pathRequest == _locations[i].getPath()) {
                location = _locations[i];
                location_found++;
                break;
            }
        }
        if (!location_found) {
            extensionPos = pathRequest.find_last_of('/');
            if (extensionPos != std::string::npos) {
                pathRequest = pathRequest.substr(0, extensionPos);
            } else {
                location = _locations[0];
                location_found++;
            }
        }
    }

    return location;
}