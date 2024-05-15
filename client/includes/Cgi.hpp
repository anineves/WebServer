#ifndef CGI_HPP
# define CGI_HPP

# include <map>
# include <string>
# include "./Request.hpp"
# include <sys/types.h>
# include <sys/wait.h>

class Request;

class Cgi {
	
	private:
		std::vector<std::string> 	_env;
		std::string					_cgi_response;
		std::string					_file_path;
		char**						_exec_env;
		char**						_cgi_argv;
		
		void	buildCharEnv(void);
		void	initEnv(Request &request);
		size_t	readCgiResponse(int fd);
	
	public:
		Cgi();
		Cgi(std::string _file_path);
		~Cgi();

		std::string runCgi(Request &request);
};


#endif