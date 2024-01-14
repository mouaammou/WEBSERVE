/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:07:10 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/14 20:12:48 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/pollServers.hpp"
#include "includes/webserv.hpp"

int main(int ac, char **av)
{
	// signal(SIGPIPE, SIG_IGN);
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
