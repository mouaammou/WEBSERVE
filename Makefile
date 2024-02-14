# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/08/01 18:37:10 by mouaammo          #+#    #+#              #
#    Updated: 2024/02/14 15:26:26 by mouaammo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

FLAGS 		= #-Wall -Wextra -Werror #-g -fsanitize=address
VERSION 	= -std=c++98
NAME 		= webserv
CCPP 		= c++
RM 			= rm -fr

RES_FILES 	= $(addprefix server/Response/src/, AutoIndex.cpp Codes.cpp MediaTypes.cpp   \
				Response.cpp  SendResponse.cpp UploadFiles.cpp )

FILES 		=  $(RES_FILES) $(addprefix server/Request/sources/, Request.cpp Method.cpp) \
				main.cpp $(addprefix server/config/src/, config.cpp directives.cpp location.cpp)
FILES 		+=  $(addprefix server/cgi/src/, NewCGI.cpp Execute.cpp ParseCGIOutput.cpp )

FILES 		+=  $(addprefix server/socket/src/, Server.cpp PollServers.cpp )


RES_HEADERS = $(addprefix server/Response/include/, AutoIndex.hpp Codes.hpp  MediaTypes.hpp  \
				Response.hpp  SendResponse.hpp UploadFiles.hpp )

HEADER_FILES 	= $(RES_HEADERS) $(addprefix server/Request/includes/, Request.hpp Method.hpp) \
					 $(addprefix server/config/include/, config.hpp directives.hpp location.hpp)

HEADER_FILES 		+=  $(addprefix server/cgi/include/, NewCGI.hpp Execute.hpp ParseCGIOutput.hpp )

HEADER_FILES 	+=  $(addprefix server/socket/include/, Server.hpp PollServers.hpp webserv.hpp)

OBJECT_FILES = $(FILES:%.cpp=%.o)

all: $(NAME)

$(NAME): $(OBJECT_FILES) $(HEADER_FILES)
	@$(CCPP) $(VERSION) $(FLAGS)  $(OBJECT_FILES) -o $(NAME)
	@echo "\033[35mWEBSEVER IS READY\033[0m";

%.o : %.cpp $(HEADER_FILES)
	@$(CCPP) $(VERSION) $(FLAGS) -c $< -o $@
	@echo "\033[1;36m Compiling \033[0m $< \033[1;36m√\033[0m";

clean:
	@$(RM) $(OBJECT_FILES)
	@echo "\033[31mOBJECT FILES DELETED\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[31mEXECUTABLE DELETED\033[0m"

re: fclean all

.PHONY: all fclean clean re
