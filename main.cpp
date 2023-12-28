/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:07:10 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/28 23:07:28 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/pollServers.hpp"
#include "includes/Server.hpp"
#include "includes/webserv.hpp"
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
		Config config_file(av[1]);
		config_file.summarize();
		PollServers pollServers(config_file);
		pollServers.initPoll();
	}
	catch (std::exception &e)
	{
		std::cout << COLOR_RED "ERROR: " << e.what() << std::endl;
	}
    return (0);
}
