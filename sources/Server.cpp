/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 00:41:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/06 17:04:04 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <sys/_types/_size_t.h>
#include <sys/poll.h>

Server::Server(std::string port):ClientRequest("")//constructor
{
    this->severPort = port;
    this->serverSocket = -1;
	this->result = NULL;
	this->sendBytes = 0;
	this->requestValuesVector.resize(0);
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

int Server::get_file_size(int fd)
{
    int file_size;
    int current_position = lseek(fd, 0, SEEK_CUR); // Get the current position
    file_size = lseek(fd, 0, SEEK_END); // Seek to the end of the file
    lseek(fd, current_position, SEEK_SET); // Return to the original position
    return file_size;
}

double	Server::bitToMegaBit(double bytes)
{
	return (bytes / 1000000);
}

void   Server::pollEvents()//rename this function: 
{
	//LISTEN:
	this->listenForConnections();//socket(), bind(), listen()
	this->videoFd = open("/goinfre/mouaammo/5.mp4", O_RDWR);
	if (this->videoFd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	this->videoSize = this->get_file_size(this->videoFd);
	
	this->responseHeader = "HTTP/1.1 200 OK\r\n";
	this->responseHeader += "Content-Type: video/mp4\r\n";
	this->responseHeader += "Content-Length: " + std::to_string(this->videoSize) + "\r\n";
	this->responseHeader += "\r\n";
	
    int timeout = 5000;//50 seconds
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
			std::cout << COLOR_YELLOW "waiting for connections ... " COLOR_RESET << std::endl;
		}
		if ((pollFds[0].fd == this->serverSocket) && (pollFds[0].revents & POLLIN))//if the first fd is ready to read
			acceptConnections();//only accept connection from the first fd

		for (size_t i = 1; i < pollFds.size(); i++)
		{
			if (pollFds[i].revents & POLLIN)//if the client is ready to read
			{
				if (receiveRequests(this->pollFds[i], i))
					pollFds[i].events = POLLOUT;//if the data is read, the client is ready to write
			}
			else if (pollFds[i].revents & POLLOUT)//if the client is ready to write
			{
				if (sendResponse(this->pollFds[i]))//write the data
				{
					resetVideoState();
					removeFileDescriptor(this->pollFds[i].fd);
				}
			}
			if ((pollFds[i].revents & POLLHUP) || (pollFds[i].revents & POLLERR))//if the client close the connection
			{
				resetVideoState();
				removeFileDescriptor(pollFds[i].fd);
			}
		}
	}
}

void	Server::resetVideoState()
{
	lseek(this->videoFd, 0, SEEK_SET);
	this->sendBytes = 0;
	this->flagSend = false;
	std::cout << COLOR_CYAN "Video state reseted" COLOR_RESET << std::endl;
}

bool    Server::sendResponse(struct pollfd &clientFd)
{
	if ((clientFd.fd != -1))
	{
		if (!flagSend)
		{
			int sendStatus = send(clientFd.fd, this->responseHeader.c_str(), this->responseHeader.length(), 0);	
			if (sendStatus < 0)
			{
				perror("send");
				return false;
			}
			this->flagSend = true;
			return false;
		}
		char *buffer = new char[50000];
		memset(buffer, 0, 50000);//clear the buffer

		if (this->sendBytes < this->videoSize)
		{
			lseek(this->videoFd, this->sendBytes, SEEK_SET);
			int readStatus = read(this->videoFd, buffer, 50000);
			if (readStatus == 0)
			{
				clientFd.events = POLLIN;
				// resetVideoState();
				return (false);
			}
			if (readStatus < 0)
			{
				perror("read");
				return false;
			}
			int value_of_send = send(clientFd.fd, buffer, readStatus, 0);
			delete [] buffer;
			
			if (value_of_send == 0)
			{
				clientFd.events = POLLIN;
				return false;
			}
			if (value_of_send < 0)
			{
				perror("send");
				return false;
			}
			std::cout << COLOR_GREEN << "SEND: " << bitToMegaBit(this->sendBytes) << "Mg / " << bitToMegaBit(this->videoSize) << "Mg";
			std::cout << COLOR_GREEN << " -- to client :=> " << clientFd.fd << COLOR_RESET << std::endl;
			this->sendBytes += value_of_send;
		} else {
			std::cout << COLOR_GREEN << "Video sent SUCCESS to client :=> " << clientFd.fd << COLOR_RESET << std::endl;
			resetVideoState();
			return (true);
		}
	}
	return (false);
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

bool	Server::receiveRequests(struct pollfd &clientFd, int index)
{
	(void)index;
	int recvStatus;
	size_t dataLength = 0;

	char *buffer = new char[MAX_REQUEST_SIZE];
	memset(buffer, 0, MAX_REQUEST_SIZE);//clear the buffer
	
	if (ClientRequest.getIsRecvHeaders() == 0)
	{
		recvStatus = recv(clientFd.fd, buffer, MAX_REQUEST_SIZE, 0);
		if (recvStatus <= 0)
			return (false);
		this->ClientRequest = Request(buffer);
	}
	else {
		recvStatus = 0;
		if (ClientRequest.getIsRecvBody())
		{
			while (dataLength < ClientRequest.getContentLength())
			{
				recvStatus = recv(clientFd.fd, buffer, 1024, 0);
				if (recvStatus <= 0)
					return (false);
				dataLength += recvStatus;
			}
		}
	}
	
	std::cout << COLOR_GREEN << "RECV from client:: " << clientFd.fd << COLOR_RESET << std::endl;
	this->ClientRequest.displayRequestHeaders();

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
