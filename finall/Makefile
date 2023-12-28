# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/22 23:06:31 by samjaabo          #+#    #+#              #
#    Updated: 2023/12/23 01:00:49 by samjaabo         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= webserv
FLAGS	= -Wall -Wextra -Werror -std=c++98 -I./include
CC		= c++

HEADERS	= $(wildcard include/*.hpp)
SRCS	= $(wildcard src/*.cpp)
OBJS	= $(SRCS:.cpp=.o)

all: clear-terminal $(NAME) run clean

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

%.o:%.cpp $(HEADERS)
	@$(CC) $(FLAGS) -c $< -o $@
	@printf "\033[1m\033[2m\033[3mcompiling... $<\033[0m\n"

$(NAME): $(OBJS)
	# @$(CC) $(FLAGS) $(OBJS) -o $(NAME)

# usage:
# 	@printf "\033[1m\033[2m\033[3mUSAGE: ./$(NAME)\033[0m\n"

clear-terminal:
	@echo "\033c"
	@printf "\033[1m\033[2m\033[3m-----------START-----------\033[0m\n"

run:
	@./$(NAME)

.PHONY: all clean fclean re