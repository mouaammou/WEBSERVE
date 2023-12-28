# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/01 18:37:10 by mouaammo          #+#    #+#              #
#    Updated: 2023/12/28 23:18:54 by mouaammo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS 		= 	-Wall -Wextra -Werror -g -fsanitize=address
VERSION 	= 	-std=c++98
NAME 		= 	webserver
CCPP 		= 	c++
RM 			= 	rm -fr

RES_FILES = $(addprefix Response/src/, AutoIndex.cpp CGI.cpp  Codes.cpp  MediaTypes.cpp  ParseCGIOutput.cpp  PipeStream.cpp  Response.cpp  SendResponse.cpp)
RES_HEADERS = $(addprefix Response/include/, AutoIndex.hpp CGI.hpp  Codes.hpp  MediaTypes.hpp  ParseCGIOutput.hpp  PipeStream.hpp  Response.hpp  SendResponse.hpp)

FILES 		=  $(RES_FILES) $(addprefix sources/, Server.cpp Request.cpp PollServers.cpp Method.cpp) main.cpp $(addprefix config/, config.cpp directives.cpp location.cpp)
HEADER_FILES 	= $(RES_HEADERS) $(addprefix includes/, Server.hpp Request.hpp PollServers.hpp webserv.hpp Method.hpp) $(addprefix config/, config.hpp directives.hpp location.hpp) Makefile

CONFIG = ./config/configfile.conf

OBJECT_FILES = $(FILES:.cpp=.o)

all: $(NAME)

run:
	./$(NAME) $(CONFIG)
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
