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

# define SERVER_PORT 8080
# define BUFFER_SIZE 30200
# define MAX_CLIENTS 20
# define MAX_EVENTS 20
# define MAXEPOLLSIZE SOMAXCONN


#endif