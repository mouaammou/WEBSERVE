/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oneServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/02 11:09:47 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/02 11:45:37 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/oneServer.hpp"
#include <sys/fcntl.h>
#include <sys/socket.h>

oneServer::oneServer(std::string port):ClientRequest("")//constructor
{
    this->severPort = port;
    this->serverSocket = -1;
	this->result = NULL;
	this->bytesSent = 0;
}

oneServer::~oneServer()//close server socket
{
    close(this->serverSocket);
}

void	oneServer::setServerSocket()
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

void    oneServer::bindServerSocket()
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

void  oneServer::listenForConnections()
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
    std::cout << COLOR_BLUE << " Server is listening on port " << COLOR_RESET<< this->severPort << std::endl;
}

int get_file_size(int fd) {
    int file_size;
    int current_position = lseek(fd, 0, SEEK_CUR); // Get the current position
    file_size = lseek(fd, 0, SEEK_END); // Seek to the end of the file
    lseek(fd, current_position, SEEK_SET); // Return to the original position
    return file_size;
}

void   oneServer::pollEvents()//rename this function: 
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
	int clientSocket;
	struct sockaddr clientAddr;
	socklen_t clientAddrLen = sizeof(clientAddr);
	if ((clientSocket = accept(this->serverSocket, &clientAddr, &clientAddrLen)) == -1)
	{
		perror("accept");
		return;
	}
	std::cout << COLOR_RED "New client connected :=> " COLOR_RESET<< clientSocket << std::endl;
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);//set the socket to non-blocking
	receiveRequests(clientSocket);
    while (true)
    {
		//accept
		//add the new client socket to the pollfd array
		//send video file
		if (sendResponse(clientSocket))
			break;
	}
	close(clientSocket);
}


bool    oneServer::sendResponse(int clientFd)
{
	//send responses to clients
	if ((clientFd != -1))
	{
		int sendStatus;
		if (this->bytesSent == 0)
		{
			sendStatus = send(clientFd, this->responseVideo.c_str(), this->responseVideo.length(), 0);	
			if (sendStatus < 0)
			{
				perror("send");
				return false;
			}
		}
		//send video file
		char *buffer = new char[10000];//clear the buffer
		int readStatus;
		while (this->bytesSent < this->videoSize)
		{
			memset(buffer, 0, 10000);
			readStatus = read(this->videoFd, buffer, 10000);
			sendStatus = send(clientFd, buffer, 10000, 0);
			if (sendStatus < 0)
			{
				perror("send");
				return false;
			}
			this->bytesSent += sendStatus;
		}
		if (this->bytesSent >= this->videoSize)
		{
			this->bytesSent = 0;
			std::cout << COLOR_GREEN << "Video sent to client :=> " << clientFd << COLOR_RESET << std::endl;
			close(this->videoFd);
			return true;
		}
		else
		{
			std::cout << COLOR_CYAN << "Video still sending ... :=> " << clientFd << COLOR_RESET << std::endl;
		}
	}
	return false;
}


bool    oneServer::receiveRequests(int &fd)
{
	char *buffer = new char[1024];

	memset(buffer, 0, 1024);//clear the buffer
	int bytes = recv(fd, buffer, 1024, 0);
	if (bytes <= 0)
		return (false);
	std::cout << COLOR_BLUE " recieve request from client :=> " COLOR_RESET<< fd << std::endl;
	std::cout << COLOR_YELLOW<< "BUFFER =:> " << buffer << COLOR_RESET << std::endl;
	return (true);
}




