#ifndef SERVER_HPP
# define SERVER_HPP
# include <vector>
# include "Location.hpp"
# include "Utils.hpp"
# include "./webServer.hpp"

class Server {
    private:
        std::string s_ip_address;
        int         s_port;
        std::string s_root;
        std::string s_index;
        std::string s_error_page;
        std::string _upload_to;
        std::vector<std::string> _methods;
        std::string _cgi_path;
        std::string _cgi_ext;
        bool        _auto_index;
        bool        _executable;
        std::vector <Location> _locations;

    public:
        Server();
        Server(std::string ipAddr, int port, std::string root, std::string index);
        ~Server();

        // ----  SETTERS  ----
        void    setIpAddr_s(std::string ipAddr);
        void    setPort_s(int port);
        void    setRoot_s(std::string root);
        void    setIndex_s(std::string index);
        void    setErrorPage_s(std::string error_page);
        void    setLocation(std::vector<Location> locations);
        void    setUploadTo(std::string upload_to);
        void    setMethods(std::vector<std::string> methods);
        void    setCgiPath(std::string cgi_path);
        void    setCgiExt(std::string cgi_ext);
        void    setAutoIndex(std::string autoindex);
        void    setExecutable(std::string executable);


        // ---- GETTERS ----
        std::string getIpAddr_s();
        int         getPort_s();
        std::string getRoot_s();
        std::string getIndex_s();
        std::string getErrorPage_s();
        std::vector<Location> getLocations();
        std::vector<std::string> getMethods_s();

        void verificErrorServer();
};

#endif