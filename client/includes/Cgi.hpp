#ifndef CGI_HPP
# define CGI_HPP

# define CGI_VERSION	"CGI/1.1"

# include <map>
# include <string>
# include "./Request.hpp"
# include <sys/types.h> // waitpid
# include <sys/wait.h> // waitpid

class Request;

class Cgi {
	private:
		std::vector<std::string> _env;
		char**								_exec_env;
		char**								_cgi_argv;
		std::string							_file_path;
	public:
		Cgi();
		Cgi(std::string _file_path);
		~Cgi();

		void	initEnv(Request &request);
		void	buildCharEnv(void);
		void	execute(void);

};


#endif