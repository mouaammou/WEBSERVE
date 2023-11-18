# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/01 18:37:10 by mouaammo          #+#    #+#              #
#    Updated: 2023/11/18 18:12:47 by mouaammo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS 		= 	-Wall -Wextra -Werror
VERSION 		= 	-std=c++98
NAME 		= 	webserver
CCPP 		= 	c++
RM 			= 	rm -fr

FILES 		= $(addprefix sources/, Server.cpp)  main.cpp 
HEADER_FILES 	= $(addprefix includes/, Server.hpp)

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
