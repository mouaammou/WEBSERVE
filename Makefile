# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/01 18:37:10 by mouaammo          #+#    #+#              #
#    Updated: 2023/12/15 17:10:00 by mouaammo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS 		= 	-Wall -Wextra -Werror -g -fsanitize=address
VERSION 	= 	-std=c++98
NAME 		= 	webserver
CCPP 		= 	c++
RM 			= 	rm -fr

FILES 		= $(addprefix sources/, Server.cpp Request.cpp PollServers.cpp ) main.cpp $(addprefix config/, config.cpp directives.cpp location.cpp)
HEADER_FILES 	= $(addprefix includes/, Server.hpp Request.hpp PollServers.hpp webserv.hpp) $(addprefix config/, config.hpp directives.hpp location.hpp) Makefile

PORT = 5555

OBJECT_FILES = $(FILES:.cpp=.o)

all: $(NAME)

run:
	./$(NAME) $(PORT)
$(NAME): $(OBJECT_FILES)
	$(CCPP) $(VERSION) $(FLAGS)  $(OBJECT_FILES) -o $(NAME)


%.o:%.cpp $(HEADER_FILES)
	$(CCPP) $(VERSION) $(FLAGS) -c $< -o $@

clean:
	$(RM) $(OBJECT_FILES)

fclean: clean
	$(RM) $(NAME)

re: fclean all
.PHONY: all fclean clean re
