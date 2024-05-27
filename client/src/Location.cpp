#include "../includes/Location.hpp"

Location::Location() : _allow_methods() {
    _path = "";
    _upload_to = "";
    _cgi_path = "";
    _cgi_ext = "";
    _autoindex ="";
    _return = "";
    _root = "";
}

Location::Location(const Location &source)
{
    *this= source;
    return ;
}


Location &Location::operator= (const Location &rhs)
{
    if(this != &rhs)
    {
       _path = rhs._path;
       _upload_to = rhs._upload_to;
       _cgi_path = rhs._cgi_path;
       _cgi_ext = rhs._cgi_ext;
       _autoindex = rhs._autoindex;
       _root = rhs._root;
       if (!rhs._allow_methods.empty()) 
        {
            _allow_methods = rhs._allow_methods;
        } 
       _return = rhs._return;
    
    }
    return *this;

}



Location::~Location() {
    _allow_methods.clear();
}

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

void Location::setRoot(std::string root)
{ 
    this->_root = root; 
}

std::string Location::getPath() const
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

std::string Location::getRoot() 
{ 
    return this->_root; 
}

std::string Location::getAutoIndex() 
{ 
    return this->_autoindex; 
}

std::string Location::getReturn() 
{ 
    return this->_return;
}

