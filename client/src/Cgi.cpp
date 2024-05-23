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
	// this->_file_path = file_path;
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

size_t Cgi::readCgiResponse(int fd)
{
	char buffer[1024];
	ssize_t bytesRead;

	while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0)
	{
		this->_cgi_response.append(buffer, bytesRead);
	}
	return this->_cgi_response.length();
}

std::string Cgi::runCgi(Request &request)
{
	int fdResponse[2];
	int fdRequest[2];
	size_t content_length;
	std::string body;
	std::stringstream resposta;

	initEnv(request);
	body = request.getFullRequest();

	if (pipe(fdResponse) == -1 || pipe(fdRequest) == -1)
		return "";

	pid_t pid = fork();
	if (pid > 0)
	{
		close(fdResponse[1]);
		close(fdRequest[0]);

		write(fdRequest[1], body.c_str(), body.length());
		close(fdRequest[1]);
		
		// Aqui começa o Timeout
		fd_set readfds;
		struct timeval timeout;
		FD_ZERO(&readfds);
		FD_SET(fdResponse[0], &readfds);
		timeout.tv_sec = 5;
		timeout.tv_usec = 0;

		// Talvez seja bom tratar -1!
		int ret = select(fdResponse[0] + 1, &readfds, NULL, NULL, &timeout);
		if (ret == 0)
		{
			std::stringstream error_resp;

			close(fdResponse[0]);
			kill(pid, SIGKILL);
			waitpid(pid, NULL, 0);

			std::string my_response = "<html lang='en'>\
										<head>\
											<meta charset='UTF-8'>\
											<meta name='viewport' content='width=device-width, initial-scale=1.0'>\
											<title>Test Page</title>\
										</head>\
										<body>\
											<h1>Hello Fuckers</h1>\
											<p>Why did you put a loop in the script?</p>\
										</body>\
										</html>";

			int length_response = my_response.length();

			std::cout << "My response:" << my_response << std::endl;
			std::cout << "Length:" << length_response << std::endl;

			error_resp << "HTTP/1.1 200 OK\r\n "; // Se por 408 não funciona.
            error_resp << "Content-Type: text/html\r\n ";
			error_resp << "Content-Length: ";
			error_resp << "328\r\n\r\n";
			error_resp << my_response;
			return error_resp.str();
				
		}

		// Aqui termina o timeout

		content_length = readCgiResponse(fdResponse[0]);

		close(fdResponse[0]);
		waitpid(pid, NULL, 0);

		int index = this->_cgi_response.find("Content-Length:");
		
		resposta << this->_cgi_response;
		resposta.seekp(index + 15);
		resposta << content_length;
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
		return "";
}