NAME = webserv
CXXFLAGS = -Werror -Wextra -Wall -std=c++98 -fsanitize=address
CXX = c++
RM = rm -rf

SRC = src/MainServer.cpp \
	src/TcpServer2.cpp \
	src/ConfigFile.cpp \
	src/Server.cpp \
	src/Utils.cpp \
	src/Request.cpp \
	src/Location.cpp \
	src/Response.cpp

OBJ = $(SRC:.cpp=.o)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)

clean :
	$(RM) $(OBJ) $(NAME)

fclean : clean
	$(RM) $(OBJ) $(NAME)

re : fclean all

.SILENT:
