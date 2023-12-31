# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/01 18:37:10 by mouaammo          #+#    #+#              #
#    Updated: 2023/12/31 05:10:24 by mouaammo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS 		= 	-Wall -Wextra -Werror -g -fsanitize=address
VERSION 	= 	-std=c++98
NAME 		= 	webserver
CCPP 		= 	c++
RM 			= 	rm -fr

RES_FILES 	= $(addprefix Response/src/, AutoIndex.cpp CGI.cpp  Codes.cpp MediaTypes.cpp  ParseCGIOutput.cpp  PipeStream.cpp  Response.cpp  SendResponse.cpp)
RES_HEADERS = $(addprefix Response/include/, AutoIndex.hpp CGI.hpp  Codes.hpp  MediaTypes.hpp  ParseCGIOutput.hpp  PipeStream.hpp  Response.hpp  SendResponse.hpp)

FILES 		=  $(RES_FILES) $(addprefix sources/, Server.cpp Request.cpp PollServers.cpp Method.cpp) \
				main.cpp $(addprefix config/, config.cpp directives.cpp location.cpp)
HEADER_FILES 	= $(RES_HEADERS) $(addprefix includes/, Server.hpp Request.hpp PollServers.hpp webserv.hpp Method.hpp) \
					 $(addprefix config/, config.hpp directives.hpp location.hpp) Makefile

CONFIG = ./config/configfile.conf

OBJECT_FILES = $(FILES:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJECT_FILES) Makefile
	@$(CCPP) $(VERSION) $(FLAGS)  $(OBJECT_FILES) -o $(NAME)
	@echo "\033[32mWEBSEVER IS READY\033[0m"


%.o:%.cpp $(HEADER_FILES)
	$(CCPP) $(FLAGS) $(VERSION) -c $< -o $@

clean:
	@$(RM) $(OBJECT_FILES)
	@echo "\033[31mOBJECT FILES DELETED\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[31mEXECUTABLE DELETED\033[0m"

re: fclean all
.PHONY: all fclean clean re
