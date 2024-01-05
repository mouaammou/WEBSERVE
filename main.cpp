/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:07:10 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/05 13:59:05 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/pollServers.hpp"
#include "includes/webserv.hpp"

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
		std::cout << COLOR_RED " " << e.what() << std::endl;
	}
    return (0);
}
