# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/01 18:37:10 by mouaammo          #+#    #+#              #
#    Updated: 2024/01/13 20:08:08 by samjaabo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS 		= -Wall -Wextra -Werror #-g -fsanitize=address
VERSION 	= -std=c++98
NAME 		= webserver
CCPP 		= c++
RM 			= rm -fr

RES_FILES 	= $(addprefix Response/src/, AutoIndex.cpp CGI.cpp Config.cpp  Codes.cpp MediaTypes.cpp  ParseCGIOutput.cpp CacheControl.cpp  \
				PipeStream.cpp  Response.cpp  SendResponse.cpp Execute.cpp UploadFiles.cpp )
RES_HEADERS = $(addprefix Response/include/, AutoIndex.hpp CGI.hpp Config.hpp  Codes.hpp  MediaTypes.hpp  ParseCGIOutput.hpp  CacheControl.hpp \
				PipeStream.hpp  Response.hpp  SendResponse.hpp Execute.hpp UploadFiles.hpp )

FILES 		=  $(RES_FILES) $(addprefix sources/, Server.cpp Request.cpp PollServers.cpp Method.cpp) \
				main.cpp $(addprefix config/, config.cpp directives.cpp location.cpp)
HEADER_FILES 	= $(RES_HEADERS) $(addprefix includes/, Server.hpp Request.hpp PollServers.hpp webserv.hpp Method.hpp) \
					 $(addprefix config/, config.hpp directives.hpp location.hpp)

OBJECT_FILES = $(FILES:%.cpp=%.o)

all: clean_screen $(NAME) run

$(NAME): $(OBJECT_FILES) $(HEADER_FILES)
	@$(CCPP) $(VERSION) $(FLAGS)  $(OBJECT_FILES) -o $(NAME)
	@echo "\033[35mWEBSEVER IS READY\033[0m";

%.o : %.cpp $(HEADER_FILES) Makefile
	@$(CCPP) $(VERSION) $(FLAGS) -c $< -o $@
	@printf "\033[1;36m Compiling \033[0m $< \033[1;36m√\033[0m\n";

clean:
	@$(RM) $(OBJECT_FILES)
	@echo "\033[31mOBJECT FILES DELETED\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[31mEXECUTABLE DELETED\033[0m"

re: fclean all


run:
	@./$(NAME) config/configfile.conf

clean_screen:
	@echo "\033c";

.PHONY: all fclean clean re
