/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:56:03 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/30 16:37:44 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <cstdio>
#include <fstream>

Server::Server(std::string port):ClientRequest("")//constructor
{
    this->severPort = port;
    this->serverSocket = -1;
	this->result = NULL;
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
	fcntl(fd, F_SETFL, O_NONBLOCK);//set server socket to non-blocking mode
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
    int reuse = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
    {
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

void   Server::pollEvents()//rename this function: 
{
	//LISTEN
	this->listenForConnections();//socket(), bind(), listen()
    int timeout = 10000;//10 seconds
    while (true)
    {
        int serverStatus = poll(this->pollFds.data(), this->pollFds.size(), timeout);//+1 for server socket
        if (serverStatus == -1)
        {
            perror("poll");
            exit (EXIT_FAILURE);
        }
        else if (serverStatus == 0)//timeout
            std::cout << COLOR_YELLOW "waiting for connections ..." COLOR_RESET << std::endl;
        else
        {
			for (size_t i = 0; i < this->pollFds.size(); i++)
			{
				if (this->pollFds[i].revents & POLL_IN)
				{
					if (this->pollFds[i].fd == this->serverSocket)
						this->acceptConnections();
					else
					{
						std::cout << COLOR_BLUE " recieve request from client :=> " COLOR_RESET<< this->pollFds[i].fd << std::endl;
						this->receiveRequests(this->pollFds[i]);  
						this->sendResponse(this->pollFds[i]);
					}
				}
			}
        }
		std::cout << COLOR_RED "No events"<< COLOR_RESET << std::endl;
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
	std::cout << "New client connected :=> " << clientSocket << std::endl;
	addFileDescriptor(clientSocket);
}

void	Server::removeFileDescriptor(int &fd)
{
	for (size_t i = 0; i < this->pollFds.size(); i++)
	{
		if (this->pollFds[i].fd == fd)
		{
			close(fd);
			fd = -1;
			this->pollFds.erase(this->pollFds.begin() + i);
			break;
		}
	}
}

void    Server::receiveRequests(struct pollfd &clientFd)
{
	char buffer[10000];

	memset(buffer, 0, sizeof (buffer));//clear the buffer
	int bytes = recv(clientFd.fd, buffer, sizeof(buffer), 0);
	// this->ClientRequest = Request(buffer);
	// this->ClientRequest.displayRequestHeaders();
	if (bytes > 0)
	{
		// std::cout << COLOR_YELLOW<< "BUFFER =:> " << buffer << COLOR_RESET << std::endl;
	}
	else if (bytes == -1)
	{
		perror("recv -1 ");
		removeFileDescriptor(clientFd.fd);
	}
	else if (bytes == 0)
	{
		perror("recv 0 ");
		removeFileDescriptor(clientFd.fd);
	}
}

void    Server::sendResponse(struct pollfd &clientFd)
{
	//send responses to clients
	if ((clientFd.fd != -1))
	{
		//generate response header
		std::string response = "HTTP/1.1 200 OK\r\n";
		//send Content-Type 
		response += "Content-Type: text/html\r\n";
		response += "Content-Length: 20\r\n";
		response += "\r\n";
		//send html file
		response += "<h1>hello world</h1>";
		//send response header
		send(clientFd.fd, response.c_str(), response.length(), 0);
		std::cout << COLOR_GREEN << "Response sent to client :=> " << clientFd.fd << COLOR_RESET << std::endl;
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
