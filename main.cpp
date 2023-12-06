/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 18:00:10 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/06 18:13:40 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "includes/Server.hpp"
#include <exception>
#include <iostream>

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	try {
		if (ac != 2)
		{
			std::cout << "Usage: ./webserv [port]" << std::endl;
			return (1);
		}

		Server myserver(av[1]);
		myserver.pollEvents();// here where the magic happens, the server will wait for events, and when it receives one, it will call the appropriate function
		myserver.closefds();
	} catch (std::exception &e) {
		std::cout << COLOR_RED "ERROR: " << e.what() << std::endl;
	}
	return (0);
}

