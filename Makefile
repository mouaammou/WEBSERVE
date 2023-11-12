# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/06/19 15:22:28 by samjaabo          #+#    #+#              #
#    Updated: 2023/11/12 23:07:50 by samjaabo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= webserv
FLAGS	= -Wall -Wextra -Werror -std=c++98
CC		= c++

HEADERS	= HttpServer.hpp HttpResponse.hpp HttpRequest.hpp
SRCS	= main.cpp HttpServer.cpp HttpResponse.cpp HttpRequest.cpp
OBJS	= $(SRCS:.cpp=.o)

all: clear-terminal $(NAME) usage run clean

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

%.o:%.cpp $(HEADERS)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "\033[1m\033[2m\033[3mcompiling... $<\033[0m\n"

$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(OBJS) -o $(NAME)

usage:
	@printf "\033[1m\033[2m\033[3mUSAGE: ./$(NAME)\033[0m\n"

clear-terminal:
	@echo "\033c"
	@printf "\033[1m\033[2m\033[3m-----------START-----------\033[0m\n"

run:
	@./$(NAME)

.PHONY: all clean fclean re usage
