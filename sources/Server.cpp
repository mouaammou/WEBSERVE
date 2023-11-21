/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:56:03 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/21 23:47:05 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <cstdio>
#include <sys/_types/_size_t.h>
#include <sys/poll.h>
#include <unistd.h>

Server::Server(std::string port)//
{
    this->severPort = port;
    this->serverSocket = -1;
	this->result = NULL;
	this->keepRunning = false;
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
    std::cout << "Server is listening on port " << this->severPort << std::endl;
    this->keepRunning = true;
}

void   Server::pollEvents()//rename this function: 
{
	//LISTEN
	this->listenForConnections();
    int timeout = 10000;//10 seconds
    while (this->keepRunning)
    {
        int serverStatus = poll(this->pollFds.data(), this->pollFds.size(), timeout);//+1 for server socket
        if (serverStatus == -1)
        {
            perror("poll");
            exit (EXIT_FAILURE);
        }
        else if (serverStatus == 0)//timeout
            std::cout << "No events for yet, Im waiting ..." << std::endl;
        else
        {
			for (size_t i = 0; i < this->pollFds.size(); i++)
			{
				if (this->pollFds[i].revents & POLL_IN)
				{
					if (this->pollFds[i].fd == this->serverSocket)
						this->acceptConnections();
					else
						this->receiveRequests(this->pollFds[i]);
				}
				else if (this->pollFds[i].revents & POLLOUT)//ready for writing
					this->sendResponse(this->pollFds[i]);
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
	addFileDescriptor(clientSocket);
}

void	Server::removeFileDescriptor(int &fd)
{
	for (size_t i = 0; i < this->pollFds.size(); i++)
	{
		if (this->pollFds[i].fd == fd)
		{
			this->pollFds.erase(this->pollFds.begin() + i);
			this->pollFds[i].events = POLLOUT;//change the event to POLLOUT: send responses to clients:: send() function
			close(fd);
			break;
		}
	}
}

void    Server::receiveRequests(struct pollfd &clientFd)
{
	char buffer[1024];
	for (size_t i = 0 ; i < this->pollFds.size(); i++)
	{
		if ((pollFds[i].fd == clientFd.fd) && (clientFd.revents & POLL_IN))//explain: why we need to check if the fd is the same
		{
			//receive requests from clients
			int bytes = recv(clientFd.fd, buffer, sizeof (buffer), 0);
			buffer[bytes] = '\0';
			if (bytes > 0)
				std::cout << "Received: from client: "<< i << " " << buffer << std::endl;
			else if (bytes == -1)
			{
				perror("recv");
				exit(1);
			}
			else if (bytes == 0)
				std::cout << "Client disconnected or hangout" << std::endl;
			this->removeFileDescriptor(clientFd.fd);
		}
	}
}

void    Server::sendResponse(struct pollfd &clientFd)
{
	//send responses to clients
	std::string response = "HTTP/1.1 200 OK\r\n"
						"Content-Type: text/html\r\n"
						"Content-Length: 100\r\n"
						"\r\n"
						"<html><body><h1>Hello, World!</h1></body></html>"
						"<p>This is a big response with multiple lines.</p>"
						"<p>It can contain any HTML content you want.</p>"
						"<p>Feel free to add more lines to customize it.</p>";
	send(clientFd.fd, response.c_str(), response.length(), 0);
	clientFd.events = POLL_IN;
	close(clientFd.fd);
}

void    Server::closefds()
{
    //close all the client sockets and the server socket
    for (size_t i = 0; i < this->pollFds.size(); i++)
    {
        close(this->pollFds[i].fd);
    }
}
