NAME = RPN
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98 -pedantic

SRC = main.cpp \
		RPN.cpp
	
OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
		echo "created $(NAME)"
		$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

clean:
			rm -f $(OBJ)
			echo "obj deleted"

fclean: clean 
			rm -f $(NAME)
			echo "$(NAME) deleted"

re: fclean all

.SILENT:

.PHONY: all, clean, fclean, re
