#include "../includes/Location.hpp"

Location::Location() {
    _path = "";
    //_allow_methods = "";
    _upload_to = "";
    _cgi_path = "";
    _cgi_ext = "";
    _autoindex ="";
    _return = "";
}

Location::~Location() {}

void Location::setPath(std::string path) 
{ 
    this->_path = path; 
}

void Location::setAllowMethods(std::vector<std::string> allow_methods) 
{ 
    this->_allow_methods.clear();
    this->_allow_methods = allow_methods; 
}

void Location::setAutoIndex(std::string autoindex) 
{ 
    this->_autoindex = autoindex; 
}
void Location::setUploadTo(std::string upload_to) 
{ 
    this->_upload_to = upload_to; 
}

void Location::setCgiPath(std::string cgi_path) 
{ 
    this->_cgi_path = cgi_path; 
}

void Location::setCgiExt(std::string cgi_ext)
{ 
    this->_cgi_ext = cgi_ext; 
}


void Location::setReturn(std::string returnn)
{ 
    this->_return = returnn; 
}

std::string Location::getPath() 
{ 
    return this->_path; 
}

std::vector<std::string> Location::getAllowMethods() 
{ 
    return this->_allow_methods; 
}

std::string Location::getUploadTo() 
{ 
    return this->_upload_to; 
}

std::string Location::getCgiPath() 
{ 
    return this->_cgi_path; 
}

std::string Location::getCgiExt() 
{ 
    return this->_cgi_ext; 
}

std::string Location::getAutoIndex() 
{ 
    return this->_autoindex; 
}

std::string Location::getReturn() 
{ 
    return this->_return;
}

void    Location::printLoc() {
    std::cout << "Path: ";
    if (_path != "") {
        std::cout << _path << std::endl;
    } else {
        std::cout << "Empty\n";
    }
    std::cout << "Upload_to: ";
    if (_upload_to != "") {
        std::cout << _upload_to << std::endl;
    } else {
        std::cout << "Empty\n";
    }
    std::cout << "CGI Path: ";
    if (_cgi_path != "") {
        std::cout << _cgi_path << std::endl;
    } else {
        std::cout << "Empty\n";
    }
    std::cout << "CGI Ext: ";
    if (_cgi_ext != "") {
        std::cout << _cgi_ext << std::endl;
    } else {
        std::cout << "Emptrey\n";
    }
    std::cout << "Autoindex: ";
    if (_autoindex != "") {
        std::cout << _autoindex << std::endl;
    } else {
        std::cout << "Empty\n";
    }
    std::cout << "Allow Methods: ";
    if (_allow_methods.size() > 0) {
        for (size_t i = 0; i < _allow_methods.size(); i++) {
            std::cout << _allow_methods[i] << ", ";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Empty\n";
    }
    std::cout << "Return: ";
    if (_return != "") {
        std::cout << _return << std::endl;
    } else {
        std::cout << "Empty\n";
    }
}