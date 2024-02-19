/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:41:33 by mouaammo          #+#    #+#             */
/*   Updated: 2024/02/19 04:25:06 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"
#include <sys/socket.h>

Server::Server(t_config config)//constructor
{
	this->serverSocket = -1;
	this->serverConfigFile = config;
	this->severPort = config.port;
	this->requested_location = "";
}

Server::~Server()//close server socket
{
    close(this->serverSocket);
}

void	Server::setConfiguration(t_config configfile)
{
	this->serverConfigFile = configfile;
}

void	Server::setServerSocket()
{
	//GETADDRINFO
	struct addrinfo *result, hints;
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
	hints.ai_socktype = SOCK_STREAM; //TCP
	hints.ai_flags = AI_PASSIVE;    // fill it with local IP address

	if (getaddrinfo("0.0.0.0", this->severPort.c_str(), &hints, &result) != 0)
	{
		throw std::runtime_error("getaddrinfo");
	}
    std::cout << COLOR_GREEN "      Server running: (http://localhost:" << this->severPort << ")" << COLOR_RESET << std::endl;
	//SETSOCKOPT
	this->serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (this->serverSocket == -1)
	{
		throw std::runtime_error("socket");// exit not allowed after server launched
	}
	this->result = result;
}

void    Server::bindServerSocket()
{
	//set server socket
	this->setServerSocket();
	int sendBufferSize = 1;  // Adjust the buffer size as needed
	if (setsockopt(this->serverSocket , SOL_SOCKET, SO_REUSEPORT , &sendBufferSize, sizeof(sendBufferSize)) == -1)
	{
		throw std::runtime_error("setsockopt");
	}
	//BIND
	if (bind(this->serverSocket, this->result->ai_addr, this->result->ai_addrlen) == -1)
	{
		throw std::runtime_error("bind");
	}
	freeaddrinfo(this->result);//free the linked list
}

int  Server::listenForConnections()
{
	this->bindServerSocket();

    if (listen(this->serverSocket, SOMAXCONN) == -1)
    {
		throw std::runtime_error("listen");
    }
    return (this->serverSocket);
}

bool		Server::isClient(int fd)
{
	return (this->httpClients.find(fd) != this->httpClients.end());
}

void		Server::addClient(int fd)
{
	this->httpClients[fd] = Request(fd, this->serverConfigFile);
}

void		Server::removeClient(int fd)
{
	this->httpClients.erase(fd);
}

int		Server::getServerSocket() const
{
	return (this->serverSocket);
}

void		Server::getTranslatedPath(std::string &translated_path, std::string path)//localhost:8080/adsfadsf
{
	for (size_t i = 0; i < this->serverConfigFile.server_locations.size(); i++)//localhost:8080/ferret/
	{
		std::string tmp = path;
		tmp[tmp.length() - 1] != '/' ? tmp += "/" : tmp;
		if (tmp.find(this->serverConfigFile.server_locations[i].getName()) != std::string::npos)
		{
			this->serverConfigFile.location = this->serverConfigFile.server_locations[i];
			std::string tmp = this->serverConfigFile.server_locations[i].getRoot() + path;
			if (tmp[tmp.length() - 1] == '/')
				tmp += this->serverConfigFile.server_locations[i].getIndex();
			translated_path = tmp;
			break;
		}
	}
}

void			Server::printf_t_config(t_config config_file)
{
	std::cout << COLOR_MAGENTA "==============Request===[" << config_file.request->getFd()<<"]=============== " COLOR_RESET <<  std::endl;
	std::cout << "	name:     [" << config_file.server_name << "]" << std::endl;
	std::cout << "	port:     [" << config_file.port << "]" << std::endl;
	std::cout << "	path:     [" << config_file.translated_path << "]" << std::endl;
	std::cout << "	location: [" << config_file.location.getName() << "]" << std::endl;
	std::cout << "	CGI:      [" << (config_file.cgi ? "true" : "false") << "]" << std::endl;
	std::cout << "	method:   [" << config_file.request->getMethod() << "]" << std::endl;
    //AUTOINDEX
    std::cout << "	autoindex:[" << (config_file.location.getAutoindex() ? "true" : "false") << "]" << std::endl;
}

void			Server::setStatusCode(std::string status_code)
{
	this->serverConfigFile.response_code = status_code;
}

std::string			Server::getStatusCode()//get status code
{
	return (this->serverConfigFile.response_code);
}
