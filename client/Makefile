NAME = webserv
CXXFLAGS = -Werror -Wextra -Wall -std=c++98 -g #-fsanitize=address
CXX = c++
RM = rm -rf

SRC = src/MainServer.cpp \
	src/TcpServer2.cpp \
	src/ConfigFile.cpp \
	src/Server.cpp \
	src/Utils.cpp \
	src/Request.cpp \
	src/Location.cpp \
	src/Response.cpp \
	src/Cgi.cpp

OBJ = $(SRC:.cpp=.o)

# UPLOAD_DIR	= ./cgi-bin/uploads/

all : $(NAME)
# 	@rm -fr $(UPLOAD_DIR)
# 	@mkdir $(UPLOAD_DIR)

$(NAME) : $(OBJ)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(NAME)

clean :
	$(RM) $(OBJ) $(NAME)

fclean : clean
	$(RM) $(OBJ) $(NAME)
# 	@rm -fr $(UPLOAD_DIR)

re : fclean all

.SILENT:
