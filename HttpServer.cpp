/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 10:52:46 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/21 18:51:13 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpServer.hpp"

uint32_t HttpServer::buffsize = 1024;

HttpServer::HttpServer()
{
	establishServerSocket(80);
	printServerAddress(pollfds.begin()->fd);
}

HttpServer::~HttpServer()
{
	for (size_t i = 0; i < pollfds.size(); ++i)
	{
		close(pollfds[i].fd);
	}
	pollfds.clear();
}

/*-------------implementation----------------*/

void HttpServer::establishServerSocket(uint16_t port)
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
	if (bind(fd, (sockaddr*)&data, sizeof(data)) == -1){
        perror("bind");
    }
	listen(fd, 1024);
}


void HttpServer::pollEventLoop( void )
{
	int	num;
	while (true)
	{
		num = poll(pollfds.data(), pollfds.size(), -1);
		if (pollfds.begin()->revents & POLLIN)//if the first fd is ready to read
		{
			//the first fd is the server fd
			onPollAccept(pollfds.begin()->fd);//only accept connection from the first fd
			--num;
		}
		for (size_t i=1; i<pollfds.size() && num>0; i++)
		{
			if (pollfds[i].revents & POLLHUP)//if the client close the connection
			{
				removeFd(pollfds[i].fd);
			}
			else if (pollfds[i].revents & POLLIN)//if the client is ready to read
			{
				if (onPollIn(pollfds[i].fd))//read the data
					pollfds[i].events = POLLOUT;//if the data is read, the client is ready to write
			}
			else if (pollfds[i].revents & POLLOUT)//if the client is ready to write
			{
				if (onPollOut(pollfds[i].fd))//write the data
					pollfds[i].events = POLLIN;//if the data is written, the client is ready to read
			}
			else
				continue ;
			--num;
		}
	}
}

bool HttpServer::onPollIn( int fd )
{
    char buff[buffsize];

	int n = read(fd, buff, buffsize - 1);
	if (n == -1)
		return false;
	buff[n] = 0;
	std::cout << buff << "---->"<<fd <<std::endl;
    return true;
}

bool HttpServer::onPollOut( int fd )
{
	std::string generateResponse = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 23\n\n<h1>Hello World!</h1>\r\n";
	write(fd, generateResponse.c_str(), strlen(generateResponse.c_str()));
	return true;
}

void HttpServer::onPollAccept( int fd )
{
	int cfd = accept(fd, NULL, NULL);
	if (cfd != -1)
		addNewFd(cfd);
}

void HttpServer::addNewFd( int fd )
{
	struct pollfd tmp;

	tmp.fd = fd;
	tmp.events = POLLIN;
	tmp.revents = 0;
	pollfds.push_back(tmp);
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

void HttpServer::removeFd( int fd )
{
	for (size_t i=0; i<pollfds.size(); ++i)
	{
		if (pollfds[i].fd == fd)
		{
			close(fd);
			pollfds.erase(pollfds.begin() + i);
			break ;
		}
	}
	std::cout << "-----------remove----->" << fd << "-------" << std::endl;
}

void HttpServer::printServerAddress( int fd )
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
