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
	content_length =0;
	timeout = std::time(NULL);
	this->_file_path = "." + file_path;
	std::cout << "CGI constructor called with " << file_path << std::endl;
}

Cgi::~Cgi()
{
	for (int i = 0; this->_exec_env[i]; i++)
		free(this->_exec_env[i]);
	delete[] this->_exec_env;

	for (int i = 0; this->_cgi_argv[i]; i++)
		free(this->_cgi_argv[i]);
	delete[] this->_cgi_argv;
}

void Cgi::initEnv(Request &request)
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

void Cgi::buildCharEnv(void)
{
	std::vector<std::string>::iterator it = this->_env.begin();
	int i = 0;

	this->_exec_env = new char *[this->_env.size() + 1];
	for (it = this->_env.begin(); it != this->_env.end(); it++, i++)
	{
		this->_exec_env[i] = strdup((*it).c_str());
	}
	this->_exec_env[i] = NULL;

	this->_cgi_argv = new char *[3];
	this->_cgi_argv[0] = strdup("/usr/bin/python3");
	this->_cgi_argv[1] = strdup(this->_file_path.c_str());
	this->_cgi_argv[2] = NULL;
}



void setNonBlocking(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return;
    }
    if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
    }
}

std::string Cgi::runCgi(Request &request)
{
    int fdResponse[2];
    int fdRequest[2];

    std::string body;
    std::stringstream resposta;
    int status = 0;

    initEnv(request);
    body = request.getFullRequest();

    if (pipe(fdResponse) == -1 || pipe(fdRequest) == -1)
        return "";

    pid_t pid = fork();
    if (pid > 0) 
    {
        close(fdResponse[1]);
        close(fdRequest[0]);

        setNonBlocking(fdResponse[0]);

        if (write(fdRequest[1], body.c_str(), body.size()) <= 0)
        {
            close(fdResponse[0]);
            close(fdRequest[1]);
            return "";
        }

        char buffer[1024];
        this->_cgi_response.clear();
        timeout = std::time(NULL);

        while (true)
        {
            ssize_t readBytes = read(fdResponse[0], buffer, sizeof(buffer));

            if (std::time(NULL) - timeout >= TIMEOUT_CGI)
            {
                kill(pid, SIGTERM);
                std::cout << "CGI timed out\n";
                break;
            }

            if (readBytes > 0)
            {
                this->_cgi_response.append(buffer, readBytes);
            }
            else if (readBytes == -1)
            {
                usleep(100); 
            }
            else if (readBytes == 0)
            {
                break;
            }
            else 
            {
                std::cout << "Error: Read from CGI\n";
                this->_cgi_response.clear();
                break;
            }
        }

        if (waitpid(pid, &status, 0) == -1)
        {
            std::cout << "Error: waitpid" << "\n";
            this->_cgi_response.clear();
        }

        close(fdResponse[0]);
        close(fdRequest[1]);

        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0)
            {
                request.setCode(500);
                std::cerr << " " << exit_status << std::endl;
                return "External script encountered an error.";
            }
        }
        else if (WIFSIGNALED(status))
        {
            request.setCode(508);
            return "External script encountered an error.";
        }

        size_t index = this->_cgi_response.find("Content-Length:");
        if (index != std::string::npos)
        {
            resposta << this->_cgi_response;
            resposta.seekp(index + 15);
            resposta << content_length;
        }
        else
        {
            resposta << this->_cgi_response;
        }

        content_length = 0;
        return resposta.str();
    }
    else if (pid == 0) 
    {
        close(fdResponse[0]);
        close(fdRequest[1]);

        dup2(fdRequest[0], STDIN_FILENO);
        dup2(fdResponse[1], STDOUT_FILENO);

        close(fdRequest[0]);
        close(fdResponse[1]);

        execve(this->_cgi_argv[0], this->_cgi_argv, this->_exec_env);
        std::exit(0);
    }
    else
    {
        return "";
    }
}