#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP
#include "./webServer.hpp"
#include "./Server.hpp"
#include "./Utils.hpp"
#include "./Location.hpp"
#include <vector>
#include <string>
#include <sstream>
#include <fstream>

class Server;

class ConfigFile
{
private:
    std::string _ip_address;
    int _port;
    std::string _root;
    std::string _index;
    std::string _error_page;
    std::vector<Server> _servers;
    std::vector<Location> _locations;

    std::string obtainValue(const std::string &line, const std::string &type);
    int obtainPort(std::string line);
    bool parserServer(std::vector<Location>& _locations, std::vector<Server> &_servers, std::stringstream *serverContent);
    bool parserLocation(std::string path, std::vector<Location> &_locations, std::stringstream *locationContent);
    std::string ft_trim(const std::string &line);


public:
    //--- CONSTRUCTORS && DESTRUCTOR ---
    ConfigFile(std::string conFile);
    ~ConfigFile();
    //--- GETTERS ---
    int getPort() const;
    std::string getIpAddr() const;
    std::string getRoot() const;
    std::string getIndex() const;
    std::string getErrorPage() const;
    std::vector<Server> &getServers();
    std::vector<Location>& getLocations();

    //--- SETTERS ---
    void setPort(int set_port);
    void setIpAddr(std::string set_ip_addr);
    void setRoot(std::string set_root);
    void setIndex(std::string set_index);
    void setErrorPage(std::string set_error_page);
    //--- PARSER ---
    void parser(std::string conFile);
};

#endif