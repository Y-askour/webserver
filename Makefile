NAME = webserv
CC = c++
FLAG = -Wall -Wextra -Werror -std=c++98 -g
HEADER = $(addprefix include/, Data.hpp Parsing.hpp Default_serv.hpp Server.hpp \
				 Location.hpp  Connection.hpp Request.hpp cgi.hpp)

SRCS = $(addprefix srcs/, main.cpp Parsing.cpp tokenizer.cpp lexer.cpp \
			 Data.cpp Server.cpp Default_serv.cpp Location.cpp save_data_server.cpp\
			 parse_mime_types.cpp Connection.cpp Request.cpp /cgi-bin/cgi.cpp)

OBJS = $(SRCS:.cpp=.o)
upload= /Users/yaskour/goinfre/upload/

all: $(NAME) $(upload)

$(upload):
	@mkdir $@

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
