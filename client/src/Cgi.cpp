#include "../includes/Cgi.hpp"


Cgi::Cgi() 
{
	this->_exec_env = NULL;
	this->_file_path = "";
}

Cgi::Cgi(std::string file_path) 
{
	this->_exec_env = NULL;
	this->_file_path = file_path;
	std::cout << "CGI constructor called." << std::endl;
}

Cgi::~Cgi() {

}
// Não sei bem se há get para todos estes, mas imagino que deva haver no Request.
// Por isto recebi Request como argumento.
void	Cgi::initEnv(Request &request) 
{

	std::cout << "## initEnv WAS CALLED! ##" << std::endl;

	this->_env.push_back("CONTENT_LENGTH=" +request.getContentLength());
	this->_env.push_back("CONTENT_TYPE=" + request.getContentType());
    this->_env.push_back("SCRIPT_FILENAME=" + request.getPath());
	this->_env.push_back("REQUEST_METHOD=" + request.getMethod());


	buildCharEnv();
}

void	Cgi::buildCharEnv(void)
{
	std::cout << "## buildCharEnv WAS CALLED! ##" << std::endl;

	this->_exec_env = new char*[this->_env.size() + 1];
	std::vector<std::string>::iterator it = this->_env.begin();
	int	i = 0;
	for (it = this->_env.begin(); it != this->_env.end(); it++, i++) {
		this->_exec_env[i] = strdup((*it).c_str());
	}
	this->_exec_env[i] = NULL; // Checar se esta subscrevendo alguma varíavel.
}

void	Cgi::execute(void)
{
	int		fdResponse[2];
	int		fdRequest[2];
	pid_t	pid;

	std::cout << "## EXECUTE WAS CALLED! ##" << std::endl;

	if (pipe(fdResponse) == -1 || pipe(fdRequest) == -1)
		return ; // Error

	pid = fork();
	if (pid > 0) {
		waitpid(pid, NULL, 0);
	}
	else if (pid == 0) {

		dup2(fdRequest[0], STDIN_FILENO);
		dup2(fdResponse[1], STDOUT_FILENO);
		close(fdRequest[0]);
		close(fdRequest[1]);
		close(fdResponse[0]);
		close(fdResponse[1]);
		//Ainda não entendi bem pra que serve esse segundo parâmetro;
		this->_cgi_argv = new char*[3];
		this->_cgi_argv[0] = strdup("/usr/bin/python3"); // Tenho que checar se podemos usar py3 e como é na escola.
		this->_cgi_argv[1] = strdup("./cgi-bin/upload.py");
		this->_cgi_argv[2] = NULL;

		char **teste = new char*[2];
		teste[0] = strdup("file=abobra");
		teste[1] = NULL;

		int	cod = execve("/usr/bin/python3", this->_cgi_argv, this->_exec_env );

		// Só para checar no terminal se execve retornou algum erro.
		int original_stdout_fd = dup(STDOUT_FILENO);
		dup2(original_stdout_fd, STDOUT_FILENO);

		if (cod == -1) {
    		std::cerr << "## ERROR EXECVE: " << strerror(errno) << " ##" << std::endl;
		}
	    close(original_stdout_fd);
	    std::exit(0);
	}
	else // Error
		return ;
}