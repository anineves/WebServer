#include "../includes/Cgi.hpp"

Cgi::Cgi() 
{
	this->_exec_env = NULL;
	this->_cgi_argv = NULL;
	this->_file_path = "";
	this->_cgi_response = "";

	std::cout << "CGI constructor called" << std::endl;
}

Cgi::Cgi(std::string file_path) 
{
	this->_exec_env = NULL;
	this->_cgi_argv = NULL;
	this->_cgi_response = "";
	this->_file_path = file_path;
	// this->_file_path = "." + file_path;
	std::cout << "CGI constructor called with " << file_path << std::endl;
}

Cgi::~Cgi() {
	for (int i = 0; this->_exec_env[i]; i++)
		free(this->_exec_env[i]);
	delete [] this->_exec_env;

	for (int i = 0; this->_cgi_argv[i]; i++)
		free(this->_cgi_argv[i]);
	delete [] this->_cgi_argv;
}

void	Cgi::initEnv(Request &request) 
{
	this->_env.push_back("PATH_INFO=" + this->_file_path);
	this->_env.push_back("HTTP_HOST=" + request.getHost());
	this->_env.push_back("QUERY_STRING=" + request.getQuery());
	this->_env.push_back("CONTENT_LENGTH=" + request.getContentLength());
	this->_env.push_back("CONTENT_TYPE=" + request.getContentType());
    this->_env.push_back("SCRIPT_FILENAME=" + request.getPath());
	this->_env.push_back("REQUEST_METHOD=" + request.getMethod());

	buildCharEnv();
}

void	Cgi::buildCharEnv(void)
{
	std::vector<std::string>::iterator it = this->_env.begin();
	int	i = 0;

	this->_exec_env = new char*[this->_env.size() + 1];
	for (it = this->_env.begin(); it != this->_env.end(); it++, i++) {
		this->_exec_env[i] = strdup((*it).c_str());
	}
	this->_exec_env[i] = NULL;

	this->_cgi_argv = new char*[3];
	this->_cgi_argv[0] = strdup("/usr/bin/python3");
	this->_cgi_argv[1] = strdup(this->_file_path.c_str());
	this->_cgi_argv[2] = NULL;
}

size_t	Cgi::readCgiResponse(int fd)
{
        char buffer[1024];
        ssize_t bytesRead;
        
        while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
            this->_cgi_response.append(buffer, bytesRead);
        }
        
        return this->_cgi_response.length();
}

void	Cgi::runCgi(Request &request, int client_fd)
{
	int					fdResponse[2];
	int					fdRequest[2];
	size_t 				content_length;
	std::string 		body;
	std::stringstream	header_stream;

	initEnv(request);
	body = request.getFullRequest();
	
	if (pipe(fdResponse) == -1 || pipe(fdRequest) == -1)
		return ; // Error

	pid_t pid = fork();
	if (pid > 0) {
        close(fdResponse[1]);               
        write(fdRequest[1], body.c_str(), body.length());
		close(fdRequest[1]);

		content_length = readCgiResponse(fdResponse[0]);
        header_stream << "HTTP/1.1 200 OK\r\n"
                      << "Content-Type: text/html\r\n"
                      << "Content-Length: " << content_length << "\r\n"
                      << "\r\n";

        std::string header = header_stream.str();
        write(client_fd, header.c_str(), header.length());
        write(client_fd, this->_cgi_response.c_str(), content_length);
        close(fdResponse[0]);		
		waitpid(pid, NULL, 0);
	}
	else if (pid == 0) {

		dup2(fdRequest[0], STDIN_FILENO);
		dup2(fdResponse[1], STDOUT_FILENO);

		close(fdRequest[0]);
		close(fdRequest[1]);
		close(fdResponse[0]);
		close(fdResponse[1]);

		execve(this->_cgi_argv[0], this->_cgi_argv, this->_exec_env );
	    std::exit(0);
	}
	else // Error
		return ;
}