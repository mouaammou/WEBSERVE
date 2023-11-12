/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATcpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 10:52:46 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 14:13:59 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ATcpServer.hpp"

uint32_t ATcpServer::buffsize = 1024;

ATcpServer::ATcpServer( void )
{
	establishServerSocket(80);
	printServerAddress(pollfds.begin()->fd);
}

ATcpServer::ATcpServer( ATcpServer const &other )
{
	(void)other;
	establishServerSocket(80);
}

ATcpServer& ATcpServer::operator=( ATcpServer const &other )
{
	(void)other;
	return *this;
}

ATcpServer::~ATcpServer( void )
{
	for (size_t i=0; i<pollfds.size(); ++i)
	{
		close(pollfds[i].fd);
	}
	pollfds.clear();
}

/*-------------implementation----------------*/

void ATcpServer::establishServerSocket(uint16_t port)
{
	struct sockaddr_in	data;
	std::memset(&data, 0, sizeof(data));
	data.sin_family = AF_INET;
	data.sin_port = htons(port);
	data.sin_addr.s_addr = INADDR_ANY;
	int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	addNewFd(fd);
	int reuseport = 0x1;
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &reuseport, sizeof(int));
	bind(fd, (sockaddr*)&data, sizeof(data));
	listen(fd, 1024);
	// std::cout<< "Server is listening on: 0.0.0.0:"
	// << ntohs(data.sin_port) << std::endl;
}


void ATcpServer::pollEventLoop( void )
{
	int	num;
	for (;;)
	{
		// std::cout<< "enterpoll" << std::endl;
		num = poll(pollfds.data(), pollfds.size(), -1);
		// std::cout<< "poll" << std::endl;
		if (pollfds.begin()->revents & POLLIN) // main socket fd
		{
			onPollAccept(pollfds.begin()->fd);
			--num;
		}
		for (size_t i=1; i<pollfds.size() && num>0; ++i)
		{
			if (pollfds[i].revents & POLLHUP)
			{
				removeFd(pollfds[i].fd);
			}
			else if (pollfds[i].revents & POLLIN)
			{
				if (onPollIn(pollfds[i].fd))
					pollfds[i].events = POLLOUT;
			}
			else if (pollfds[i].revents & POLLOUT)
			{
				if (onPollOut(pollfds[i].fd))
					pollfds[i].events = POLLIN;
			}
			else
				continue ;
			--num;
		}
	}
}

bool ATcpServer::onPollIn( int fd )
{
	char buff[buffsize];

	int n = read(fd, buff, buffsize - 1);
	if (n == -1)
		return false;
	buff[n] = 0;
	std::cout << buff << "---->"<<fd <<std::endl;
	buffers[fd] += buff;
	return false;
}

bool ATcpServer::onPollOut( int fd )
{
	// std::cout << "Test POLLOUT" << fd << std::endl;
	char hi[] = "HTTP/1.1 200 OK\nConnection:keep-alive\nContent-Type: text/html\nContent-Length: 23\n\n<h1>Hello World!</h1>\r\n";
				// "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!\n"
	// send(fd, hi, 76, 0);
	write(fd, hi, strlen(hi));
	// std::cout << hi << "-->" << fd << "n="<<n<<std::endl;
	return true;
}

void ATcpServer::onPollAccept( int fd )
{
	int cfd = accept(fd, NULL, NULL);
	if (cfd != -1)
		addNewFd(cfd);
	// std::cout << "-----------accept----->"<<cfd<<"-------" << std::endl;
}

void ATcpServer::addNewFd( int fd )
{
	struct pollfd tmp;

	tmp.fd = fd;
	tmp.events = POLLIN;
	tmp.revents = 0;
	pollfds.push_back(tmp);
	fcntl(fd, F_SETFL, O_NONBLOCK);
	buffers[fd] = "";
}

void ATcpServer::removeFd( int fd )
{
	std::cout << "-----------remove----->" << fd << "-------" << std::endl;
	for (size_t i=0; i<pollfds.size(); ++i)
	{
		if (pollfds[i].fd == fd)
		{
			close(fd);
			pollfds.erase(pollfds.begin() + i);
			pollfds.shrink_to_fit();
			break ;
		}
	}
	buffers.erase(fd);
}

void ATcpServer::printServerAddress( int fd )
{
	struct sockaddr localAddress;
	socklen_t addressLength = sizeof(localAddress);
	getsockname(fd, &localAddress, &addressLength);
	struct sockaddr_in* localSocketAddress = (struct sockaddr_in*)&localAddress;
	std::cout	<< "Server is runing on: "
	<< inet_ntoa(localSocketAddress->sin_addr)
	<< ":" << ntohs(localSocketAddress->sin_port)
	<< std::endl;
}