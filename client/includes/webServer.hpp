#ifndef WEBSERVER_HPP
# define WEBSERVER_HPP
# include <iostream>
# include <cstring>
# include <iostream>
# include <cstdlib>
# include <fcntl.h>
# include <fstream>
# include <sstream>
# include <unistd.h>
# include <exception>
# include <sys/socket.h> // basic socket definitions
# include <sys/sendfile.h> // basic socket definitions
# include <netinet/in.h>
# include <arpa/inet.h>
# include <cerrno>
# include <stdio.h>
# include <stdlib.h>
# include <sys/epoll.h>

// ------- variables
# define SERVER_PORT 8080
# define BUFFER_SIZE 30200
# define MAX_CLIENTS 20
# define MAX_EVENTS 20
# define MAXEPOLLSIZE SOMAXCONN

// ------- colors
# define RESET   "\033[0m"
# define BLACK   "\033[30m"
# define RED     "\033[31m"
# define GREEN   "\033[32m"
# define YELLOW  "\033[33m"
# define BLUE    "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN    "\033[36m"
# define WHITE   "\033[37m"
# define BOLD    "\033[1m"
# define BLINK    "\033[5m"


#endif