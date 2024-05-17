#ifndef SERVER_HPP
# define SERVER_HPP
# include <vector>
# include "Location.hpp"
# include "Utils.hpp"
# include "./webServer.hpp"

class Server {
    private:
        
        std::string             s_root;
        std::string             s_index;
        std::vector<std::string> s_index_pages;
        std::string             s_error_page;
        std::string             s_server_name;
        
        std::string             s_ip_address;
        std::string             s_port;
        int                     s_client_body;


        std::string             _upload_to;
        std::string             _cgi_path;
        std::string             _cgi_ext;
        bool                    _auto_index;
        bool                    _executable;
        std::string             _return;
        int                     s_socket;
        bool                    _redirect;
        socklen_t               s_socketAddress_len;

    public:
        std::vector<Location>  _locations;
        uint32_t                s_addr;
        uint16_t                sin_port;
        std::string             s_host;
        std::vector<std::string> s_server_names;
        
        struct sockaddr_in      s_socketAddress;
        //bool        _redirect;
        std::vector<std::string> _methods;
        Server();
        Server(const Server &source);
        Server &operator = (const Server &rhs);
        Server(std::string ipAddr, int port, std::string root, std::string index);
        ~Server();

        // ----  SETTERS  ----
        void    setIpAddr_s(std::string ipAddr);
        void    setPort_s(std::string port);
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
        void    setRedirect(std::string redirect);
        void    setReturn(std::string returnn);
        void    setSocket(int set_socket);
        void    setSocketAddr(struct sockaddr_in n_socketaddr);
        void    setSocketAddr_len(socklen_t n_socketaddr_len);
        void    setServerName_s(std::string cgi_ext);
        void    setClientMaxBody(int client_max_body);
        // ---- GETTERS ----
        std::string                 getIpAddr_s();
        std::string                         getPort_s();
        std::string                 getRoot_s();
        std::string                 getIndex_s();
        std::string                 getErrorPage_s();
        std::vector<Location>       getLocations();
        std::vector<std::string>    getMethods_s();
        int                         getSocket();
        struct sockaddr_in          getSocketAddr();
        socklen_t                   getSocketAddr_len();
        int                         getClientMaxBody_s();
        bool                        getExecutable();
        bool                        getRedirect();
        void    verificErrorServer();
        Location                    verifyLocations(std::string pathRequest);
       
};

#endif