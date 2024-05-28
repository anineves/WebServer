#ifndef CGI_HPP
#define CGI_HPP

#include <map>
#include <string>
#include "./Request.hpp"
#include <sys/types.h>
#include <sys/wait.h>
#include <ctime>
#include <csignal>
#include <vector>

#define TIMEOUT_CGI 4

class Request;

class Cgi {
private:
    std::vector<std::string> _env;
    std::string _cgi_response;
    std::string _file_path;
    char **_exec_env;
    char **_cgi_argv;
    std::time_t timeout;

    void buildCharEnv(void);
    void initEnv(Request &request);
    size_t readCgiResponse(int fd);

public:
    Cgi();
    Cgi(std::string _file_path);
    ~Cgi();

    std::string runCgi(Request &request);
	size_t		content_length;
	pid_t	pid;
	
};

#endif