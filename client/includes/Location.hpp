#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>

class Location {
private:
    std::string _path;
    std::string _upload_to;
    std::string _cgi_path;
    std::string _cgi_ext;
    std::string _autoindex;
    std::vector<std::string> _allow_methods;

public:
    Location();
    ~Location();

    void setPath(std::string path);
    void setAllowMethods(std::vector<std::string> allow_methods);
    void setUploadTo(std::string upload_to);
    void setCgiPath(std::string cgi_path);
    void setCgiExt(std::string cgi_ext);
    void setAutoIndex(std::string autoindex);

    std::string getPath();
    std::vector<std::string> getAllowMethods();
    std::string getUploadTo();
    std::string getCgiPath();
    std::string getCgiExt();
    std::string getAutoIndex();
};

#endif 