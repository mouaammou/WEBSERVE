/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/05 16:13:55 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/06 17:42:06 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "HttpCgi.hpp"
# include <iostream>
# include <map>
# include <vector>
# include <cstdlib>
# include <cstring>
# include <cctype>
# include <unistd.h>
# include <fcntl.h>
#include <cstdlib>
#include <csignal>
#include <sstream>
#include <poll.h>

size_t buffsize = 47;


int main( void )
{
	int fd = open("testcgi", O_RDWR|O_TRUNC);
	HttpCgi hi;
	// hi.inputfd = fd;
	hi.request_body = "helo world";
	// bool a = hi.readOutOfCgi();
	// std::cout << "error: " << hi.error << std::endl;
	// std::cout << "first: " << a << std::endl;
	// bool b = hi.sendBodyToCgi();
	// std::cout << "error: " << hi.error << std::endl;
	// std::cout << "second: " << b << std::endl;
	std::cout << "-----body-----" << std::endl;
	std::cout << hi.buffer << std::endl;
	// struct pollfd d;
	// d.fd = fd;
	// char s[45];
	// poll(&d, 1, -1);
	// std::cout << "revent:"<< d.revents << std::endl;
	// std::cout << "read: " << read(fd, s, 45) << std::endl;
	// poll(&d, 1, -1);
	// std::cout << "revent:"<< d.revents << std::endl;
	// std::cout << "read: " << read(fd, s, 45) << std::endl;
	return (0);
}