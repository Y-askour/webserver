NAME = webserv
CC = c++
FLAG = -Wall -Wextra -Werror -std=c++98 -g
HEADER = $(addprefix include/, parsing.hpp)

SRCS =  $(addprefix srcs/, main.cpp parsing.cpp tokenizer.cpp lexer.cpp)

OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $^ -o $@

%.o: %.cpp $(HEADER)
	$(CC) $(FLAG) -c $< -o $@

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
