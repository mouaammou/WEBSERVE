/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:41:33 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/03 11:38:11 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <string>

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
    hints.ai_socktype = SOCK_STREAM;//TCP
    hints.ai_flags = AI_PASSIVE;    // fill it with local IP address

    if (getaddrinfo(NULL, this->severPort.c_str(), &hints, &result) != 0)
    {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }
    //SETSOCKOPT
    this->serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (this->serverSocket == -1)
    {
        perror("socket");
        exit (EXIT_FAILURE);// exit not allowed after server launched
    }
	this->result = result;
}

void    Server::bindServerSocket()
{
	//set server socket
	this->setServerSocket();
	int sendBufferSize = 1;  // Adjust the buffer size as needed
	if (setsockopt(this->serverSocket , SOL_SOCKET, SO_REUSEPORT , &sendBufferSize, sizeof(sendBufferSize)) == -1) {
		perror("setsockopt");
		exit (EXIT_FAILURE);
	}
    //BIND
    if (bind(this->serverSocket, this->result->ai_addr, this->result->ai_addrlen) == -1)
    {
        perror("bind");
        exit (EXIT_FAILURE);
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
        perror("listen");
        exit (EXIT_FAILURE);
    }
    return (this->serverSocket);
}

bool		Server::isClient(int fd)
{
	return (this->httpClients.find(fd) != this->httpClients.end());
}

void		Server::addClient(int fd)
{
	std::cout << COLOR_CYAN "New client" COLOR_RESET << std::endl;
	this->httpClients[fd] = new Request(fd, this->serverConfigFile);
}

void		Server::removeClient(int fd)
{
	std::cout << COLOR_RED "Client removed " << fd << COLOR_RESET << std::endl;
	delete this->serverConfigFile.request;
	delete this->pointedMethod;
	this->serverConfigFile.request = NULL;
	this->httpClients.erase(fd);
}

int		Server::getServerSocket() const
{
	return (this->serverSocket);
}

std::string			Server::getRequestedLocation(std::string path)
{
	for (size_t i = 0; i < this->serverConfigFile.server_locations.size(); i++)
	{
		if (path.find(this->serverConfigFile.server_locations[i].getName()) != std::string::npos)
		{
			return (this->serverConfigFile.server_locations[i].getName());
		}
	}
	return ("/");
}

std::string		Server::getTranslatedPath(std::string location, std::string path)
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
	std::cout << "configuration : \n";
	printf("		server_name: %s\n", config_file.server_name.c_str());
	printf("		port: %s\n", config_file.port.c_str());
	printf("		translated_path: %s\n", config_file.translated_path.c_str());
	printf("		response_code: %s\n", config_file.response_code.c_str());
	printf("		req_location: %s\n", config_file.requested_path.c_str());
	printf("		autoindex: %s\n", config_file.autoindex.c_str());
	printf("		server_fd: %d\n", config_file.server_fd);
	printf("		body_size: %d\n", config_file.body_size);
	printf("		Location: %s\n", config_file.location.getName().c_str());
	printf("		iscgi: %s\n", config_file.cgi ? "true" : "false");
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
