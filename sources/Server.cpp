/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:41:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/08 13:09:14 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <sys/_types/_size_t.h>
#include <sys/poll.h>

Server::Server(std::string port)//constructor
{
    this->severPort = port;
	this->serverSocket = -1;
}

Server::~Server()//close server socket
{
    close(this->serverSocket);
}

void	Server::addFileDescriptor(int fd)
{
	struct pollfd newFd;

	newFd.fd = fd;
	newFd.events = POLL_IN;
	this->pollFds.push_back(newFd);
	fcntl(newFd.fd , F_SETFL, O_NONBLOCK);//set server socket to non-blocking mode
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
        exit (EXIT_FAILURE);
    }
	this->result = result;
}

void    Server::bindServerSocket()
{
	//set server socket
	this->setServerSocket();
	int sendBufferSize = 1;  // Adjust the buffer size as needed
	if (setsockopt(this->serverSocket , SOL_SOCKET, SO_REUSEADDR, &sendBufferSize, sizeof(sendBufferSize)) == -1) {
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

void  Server::listenForConnections()
{
	//BIND
	this->bindServerSocket();
    //LISTEN
    if (listen(this->serverSocket, 1024) == -1)
    {
        perror("listen");
        exit (EXIT_FAILURE);
    }
    //ADD SERVER SOCKET TO POLLFDS
	addFileDescriptor(this->serverSocket);
    std::cout << COLOR_BLUE << " Server is listening on port " << COLOR_RESET<< this->severPort << std::endl;
}

double	Server::bitToMegaBit(double bytes)
{
	return (bytes / 1000000);
}

void   Server::pollEvents()//rename this function: 
{
	//LISTEN:
	this->listenForConnections();//socket(), bind(), listen()
	//POLL
    int timeout = 5000;//50 seconds
	int pollStatus;

	int pageFd = open("/goinfre/mouaammo/vvvv.mp4", O_RDONLY);
	if (pageFd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
    while (true)
    {
		pollStatus = poll(this->pollFds.data(), pollFds.size(), timeout);
		if (pollStatus == -1)
		{
			perror("poll");
			exit(EXIT_FAILURE);
		}
		else if (pollStatus == 0)
		{
			std::cout << COLOR_YELLOW "waiting for connections ... " COLOR_RESET << std::endl;
		}
		if ((pollFds[0].fd == this->serverSocket) && (pollFds[0].revents & POLLIN))//if the first fd is ready to read
			acceptConnections();//only accept connection from the first fd

		for (size_t i = 1; i < pollFds.size(); i++)
		{
			if (pollFds[i].revents & POLLIN)//if the client is ready to read
			{
				if (this->httpClients[pollFds[i].fd]->receiveRequest())//receive the request
				{
					this->pollFds[i].events = POLLOUT;//set the client to write
					this->httpClients[pollFds[i].fd]->displayRequest();
					this->httpClients[pollFds[i].fd]->resetRequestState();//reset the client
					this->httpClients[pollFds[i].fd]->resetResponseState();//reset the client
					continue;
				}
			}
			else if (pollFds[i].revents & POLLOUT)//if the client is ready to write
			{
				std::cout << COLOR_GREEN "Client is ready to write" COLOR_RESET << std::endl;
				if (this->httpClients[pollFds[i].fd]->sendResponse(pageFd))//send the response
				{
					this->pollFds[i].events = POLLIN;//set the client to read
					this->httpClients[pollFds[i].fd]->resetRequestState();//reset the client
					this->httpClients[pollFds[i].fd]->resetResponseState();//reset the client
					this->removeClient(pollFds[i].fd);//remove the client from the pollfd array
					break;
				}
			}
			if ((pollFds[i].revents & POLLHUP) || (pollFds[i].revents & POLLERR))//if the client close the connection
			{
				removeClient(pollFds[i].fd);//remove the client from the pollfd array
			}
		}
	}
}

void   Server::acceptConnections()
{
    struct sockaddr clientAddr;
    int clientSocket;
    socklen_t clientAddrLen = sizeof(clientAddr);

    if ((clientSocket = accept(this->serverSocket, &clientAddr, &clientAddrLen)) == -1)
    {
        perror("accept");
        return;
    }
    //add the new client socket to the pollfd array
	std::cout << COLOR_RED "New client connected :=> " COLOR_RESET<< clientSocket << std::endl;
	addNewClient(clientSocket);
}

void	Server::addNewClient(int fd)
{
	addFileDescriptor(fd);
	Client *newClient = new Client(fd);
	this->httpClients.insert(std::pair<int, Client*>(fd, newClient));
}

void	Server::removeClient(int fd)
{
	removeFileDescriptor(fd);
	this->httpClients.erase(fd);
}


void	Server::removeFileDescriptor(int fd)
{
	for (size_t i = 0; i < this->pollFds.size(); i++)
	{
		if (this->pollFds[i].fd == fd)
		{
			std::cout << COLOR_RED "Client disconnected :=> " COLOR_RESET<< fd << std::endl;
			close(fd);
			fd = -1;
			this->pollFds.erase(this->pollFds.begin() + i);
			break;
		}
	}
}

void    Server::closefds()
{
//close all the client sockets and the server socket
	for (size_t i = 0; i < this->pollFds.size(); i++)
	{
		close(this->pollFds[i].fd);
	}
}
