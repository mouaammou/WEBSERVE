/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:41:33 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/24 18:08:43 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Server.hpp"

Server::Server(t_config config)//constructor
{
	this->serverSocket = -1;
	this->serverConfigFile = config;
	this->severPort = config.port;
	this->pointedMethod = NULL;
	this->requested_location = "";
}

Server::~Server()//close server socket
{
	for (std::map<int, Request*>::iterator it = this->httpClients.begin(); it != this->httpClients.end(); it++)
	{
		delete it->second;
		it->second = NULL;
	}
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
	//BIND
	this->bindServerSocket();
    //LISTEN
    if (listen(this->serverSocket, 1024) == -1)
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
	this->httpClients[fd] = new Request(fd, this->serverConfigFile);
}

void		Server::removeClient(int fd)
{
	delete this->httpClients[fd];
	this->httpClients[fd] = NULL;
	this->httpClients.erase(fd);
}

int		Server::getServerSocket() const
{
	return (this->serverSocket);
}

std::string			Server::getRequestedLocation(std::string path)
{
	for (size_t i = 0; i < this->serverConfigFile.server_locations.size(); i++)//localhost:8080/ferret/
	{
		std::string tmp = path;
		tmp[tmp.length() - 1] != '/' ? tmp += "/" : tmp;
		if (tmp.find(this->serverConfigFile.server_locations[i].getName()) != std::string::npos)
		{
			this->serverConfigFile.location = this->serverConfigFile.server_locations[i];
			return (this->serverConfigFile.server_locations[i].getName());
		}
	}
	return ("/");
}

std::string		Server::getTranslatedPath(std::string location, std::string path)//localhost:8080/adsfadsf
{
	size_t i;
	for (i = 0; i < this->serverConfigFile.server_locations.size(); i++)
	{
		if (this->serverConfigFile.server_locations[i].getName() == location)
			break;
	}
	std::string tmp = this->serverConfigFile.server_locations[i].getRoot() + path;
	if (tmp[tmp.length() - 1] == '/')
		tmp += this->serverConfigFile.server_locations[i].getIndex();
	return (tmp);
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
}

Method* 		Server::getPointedMethod() const
{
	return (this->pointedMethod);
}

void			Server::setStatusCode(std::string status_code)
{
	this->serverConfigFile.response_code = status_code;
}

std::string			Server::getStatusCode()//get status code
{
	return (this->serverConfigFile.response_code);
}
