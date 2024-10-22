/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:07:10 by mouaammo          #+#    #+#             */
/*   Updated: 2024/02/18 20:30:58 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/socket/include/pollServers.hpp"
#include "server/socket/include/webserv.hpp"

bool	endsWithConf( std::string str )
{
	size_t strLen = str.length();
    size_t confLen = std::strlen(".conf");

    if (strLen >= confLen)
        return str.compare(strLen - confLen, confLen, ".conf") == 0;
    return false;
}

long getCurrentTimeInSeconds() {
    struct timeval currentTime;
    gettimeofday(&currentTime, nullptr);
    return currentTime.tv_sec;
}

int main(int ac, char **av)
{
	signal(SIGPIPE, SIG_IGN);
	try
	{
		std::string path;
		if (ac == 2 && endsWithConf(av[1]))
			path = av[1];
		else if (ac == 1)
			path = "config.conf";
		else
		{
			std::cout << "Usage: ./webserv [*.conf]" << std::endl;
			return (1);
		}
		Config config_file(path);
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
