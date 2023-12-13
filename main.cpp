/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:07:10 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/13 20:09:54 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Server.hpp"
#include <exception>
#include <iostream>

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	try
	{
		if (ac != 2)
		{
			std::cout << "Usage: ./webserv [port]" << std::endl;
			return (1);
		}
		
		Server myserver(av[1]);
		myserver.pollEvents();// here where the magic happens, the server will wait for events, and when it receives one, it will call the appropriate function
		myserver.closefds();
	}
	catch (std::exception &e)
	{
		std::cout << COLOR_RED "ERROR: " << e.what() << std::endl;
	}
    return (0);
}
