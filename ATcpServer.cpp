/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATcpServer.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 10:52:46 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 00:28:59 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ATcpServer.hpp"
#include "CGI.cpp"
#include <sys/time.h>
#include "SendResponse.cpp"
#include "ResponseGeneration.cpp"

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
	timeval startTime, endTime;
	gettimeofday(&startTime, nullptr);
	for (;;)
	{
		// std::cout<< "enterpoll" << std::endl;
		
		num = poll(pollfds.data(), pollfds.size(), 1000000);
		// CGI::checkTimeoutAndExitedProcesses();
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
	static t_config		config;

	uri = buffers[fd].substr(0, buffers[fd].find("\r\n"));
	uri.erase(0, uri.find("/"));
	uri.erase(uri.find(" "), uri.length());
	buffers[fd].clear();
	// uri = uri.substr(0, uri.find(" "));
	if (uri == "/favicon.ico")
	{
		removeFd(fd);
		return false;
	}
	// if (uri.compare(uri.rfind('.'), 3, ".py") == 0)
	// {
	// 	std::cout << "cgi uri->: " << uri << std::endl;
	// 	CGI::build(config, fd);
	// 	std::string output = Response::getClientResponse(fd);
	// 	if 
	// 	SendResponse(output, -1, fd); 
	// 	return true;
	// }
	std::cout << "uri->: " << uri << std::endl;
	AutoIndex ai(fd, "/", uri);
	std::string output = ai.getOutput();
	// CGI::build(config, fd);
	// int ffd = open("templates/index.html", O_RDONLY);
	// struct stat stat_buf;
	// fstat(ffd, &stat_buf);
	// (void)stat_buf.st_size;
	// std::stringstream response;
	// response  << "HTTP/1.1 200 OK\r\n";
	// response << "Content-Length: " << output.length() << "\r\n";
	// response  << "Content-Type: text/html\r\n\r\n";
	// response << output;
	SendResponse(output, -1, fd); 
	return true;
}

bool ATcpServer::onPollOut( int fd )
{
	// std::string response = Response::getClientResponse(fd);
	// std::cout << response;; 
	// if (response.empty())
	// {

	// 	return false;
	// }
	// // std::cout << "response: " << response << std::endl;
	// int a  = write(fd, response.c_str(), response.size());
	// std::cout << "write: " << a << std::endl;
	return SendResponse::send(fd);
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