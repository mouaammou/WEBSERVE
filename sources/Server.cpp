/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:56:03 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/02 01:44:45 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <__errc>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sys/fcntl.h>
#include <sys/poll.h>
#include <sys/signal.h>
#include <unistd.h>

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
	newFd.revents = 0;
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

int get_file_size(int fd) {
    int file_size;
    int current_position = lseek(fd, 0, SEEK_CUR); // Get the current position
    file_size = lseek(fd, 0, SEEK_END); // Seek to the end of the file
    lseek(fd, current_position, SEEK_SET); // Return to the original position
    return file_size;
}

void   Server::pollEvents()//rename this function: 
{
	//LISTEN:
	this->listenForConnections();//socket(), bind(), listen()
	this->videoFd = open("/goinfre/mouaammo/vvvv.mp4", O_RDWR);
	if (this->videoFd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	this->videoSize = get_file_size(this->videoFd);

	this->responseVideo = "HTTP/1.1 200 OK\r\n";
	this->responseVideo += "Content-Type: video/mp4\r\n";
	this->responseVideo += "Content-Length: " + std::to_string(videoSize) + "\r\n";
	this->responseVideo += "\r\n";
    int timeout = 50000;//50 seconds
	int pollStatus;
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
			std::cout << COLOR_RED "waiting for connections ... " COLOR_RESET << std::endl;
			continue ;
		}
		for (size_t i=0; i < pollFds.size(); i++)
		{
			if ((pollFds[i].fd == this->serverSocket) && (pollFds[i].revents & POLLIN))//if the first fd is ready to read
			{
				//the first fd is the server fd
				acceptConnections();//only accept connection from the first fd
				continue;
			}
			if (pollFds[i].revents & POLLIN)//if the client is ready to read
			{
				if (receiveRequests(this->pollFds[i]))//read the data
					pollFds[i].events = POLLOUT;//if the data is read, the client is ready to write
			}
			else if (pollFds[i].revents & POLLOUT)//if the client is ready to write
			{
				sendResponse(this->pollFds[i]);//write the data
				// pollFds[i].events = POLLIN;//if the data is written, the client is ready to read
			}
			if (pollFds[i].revents & POLLHUP)//if the client close the connection
				removeFileDescriptor(pollFds[i].fd);
		}
	}
}


void    Server::sendResponse(struct pollfd &clientFd)
{
	static int result = 0;
	//send responses to clients
	if ((clientFd.fd != -1))
	{
		int sendStatus;
		if (result == 0)
		{
			sendStatus = send(clientFd.fd, this->responseVideo.c_str(), this->responseVideo.length(), 0);	
			if (sendStatus < 0)
			{
				perror("send");
				return;
			}
		}
		//send video file
		char *buffer = new char[100000];
		memset(buffer, 0, 100000);//clear the buffer
		int readStatus;
		if (result < this->videoSize)
		{
			readStatus = read(this->videoFd, buffer, 100000);
			sendStatus = send(clientFd.fd, buffer, 100000, 0);
			if (sendStatus <= 0)
			{
				perror("send");
				return ;
			}
			result += sendStatus;
		}
		if (result >= this->videoSize)
		{
			clientFd.events = POLLIN;
			std::cout << COLOR_GREEN << "Video sent to client :=> " << clientFd.fd << COLOR_RESET << std::endl;
			// removeFileDescriptor(clientFd.fd);
			// exit (100);
			// close(this->videoFd);
		}
		else
		{
			clientFd.events = POLLOUT;
			std::cout << COLOR_GREEN << "Video still sending ... :=> " << clientFd.fd << COLOR_RESET << std::endl;
		}
		// close(this->videoFd);
		// removeFileDescriptor(clientFd.fd);
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
	addFileDescriptor(clientSocket);
}

void	Server::removeFileDescriptor(int &fd)
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

bool    Server::receiveRequests(struct pollfd &clientFd)
{
	char *buffer = new char[1024];

	memset(buffer, 0, 1024);//clear the buffer
	int bytes = recv(clientFd.fd, buffer, 1024, 0);
	if (bytes <= 0)
		return (false);
	std::cout << COLOR_BLUE " recieve request from client :=> " COLOR_RESET<< clientFd.fd << std::endl;
	std::cout << COLOR_YELLOW<< "BUFFER =:> " << buffer << COLOR_RESET << std::endl;
	return (true);
}

void    Server::closefds()
{
//close all the client sockets and the server socket
	for (size_t i = 0; i < this->pollFds.size(); i++)
	{
		close(this->pollFds[i].fd);
	}
}
