# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/01 18:37:10 by mouaammo          #+#    #+#              #
#    Updated: 2023/11/25 16:32:21 by mouaammo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS 		= 	-Wall -Wextra -Werror #-g -fsanitize=address
VERSION 	= 	-std=c++98
NAME 		= 	webserver
CCPP 		= 	c++
RM 			= 	rm -fr

FILES 		= $(addprefix sources/, Server.cpp Request.cpp)  main.cpp 
HEADER_FILES 	= $(addprefix includes/, Server.hpp Request.hpp) Makefile

OBJECT_FILES = $(FILES:.cpp=.o)

all: $(NAME)

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
