#include "../includes/Location.hpp"

Location::Location() {
    _path = "";
    _allow_methods = "";
    _upload_to = "";
    _cgi_path = "";
    _cgi_ext = "";
    _autoindex ="";
    
}

Location::~Location() {}

void Location::setPath(std::string path) 
{ 
    this->_path = path; 
}

void Location::setAllowMethods(std::string allow_methods) 
{ 
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

std::string Location::getPath() 
{ 
    return this->_path; 
}

std::string Location::getAllowMethods() 
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