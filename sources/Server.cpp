/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:56:03 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/19 09:52:36 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <iostream>

Server::Server(std::string port)//
{
    this->severPort = port;
    this->serverSocket = -1;
    this->serverStatus = -1;
    this->pollFds.resize(MAX_CLIENTS + 1);//+1 for server socket
}

Server::~Server()//close server socket
{
    close(this->serverSocket);
}

void    Server::startServer()
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
    fcntl(this->serverSocket, F_SETFL, O_NONBLOCK);//set server socket to non-blocking mode
    int reuse = 1;
    if (setsockopt(this->serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
    {
        perror("setsockopt");
        exit (EXIT_FAILURE);
    }
    //BIND
    if (bind(this->serverSocket, result->ai_addr, result->ai_addrlen) == -1)
    {
        perror("bind");
        exit (EXIT_FAILURE);
    }
    freeaddrinfo(result);//free the linked list
}

void  Server::fillPollFds()
{
    //LISTEN
    if (listen(this->serverSocket, MAX_CLIENTS + 1) == -1)//+1 for server socket
    {
        perror("listen");
        exit (EXIT_FAILURE);
    }
    //ADD SERVER SOCKET TO POLLFDS
    this->pollFds[0].fd = this->serverSocket;
    this->pollFds[0].events = POLL_IN;
    
    std::cout << "Server is listening on port " << this->severPort << std::endl;
    //INITIALIZE POLLFDS
    for (int i = 1; i < MAX_CLIENTS + 1; i++)
    {
        this->pollFds[i].fd = -1;
        this->pollFds[i].events = POLL_IN;
    }
}

void   Server::pollEvents()
{
    int timeout = 5000;//5 seconds
    while (true)
    {
        int serverStatus = poll(this->pollFds.data(), MAX_CLIENTS + 1, timeout);//+1 for server socket
        if (serverStatus == -1)
        {
            perror("poll");
            exit (EXIT_FAILURE);
        }
        else if (serverStatus == 0)//timeout
            std::cout << "No events for 5 seconds" << std::endl;
        else
        {
            this->serverStatus = serverStatus;
            this->acceptConnections();
            this->receiveRequests();
            this->sendResponses();
        }
    }
}

void   Server::acceptConnections()
{
    struct sockaddr clientAddr;
    int clientSocket;
    socklen_t clientAddrLen = sizeof(clientAddr);
    if (this->serverStatus > 0)
    {
        if ((clientSocket = accept(this->serverSocket, &clientAddr, &clientAddrLen)) == -1)
        {
            return;
        }
        else
        {
            //add the new client socket to the pollfd array
            for (int i = 1; i < MAX_CLIENTS + 1; i++)
            {
                if (this->pollFds[i].fd == -1)
                {
                    this->pollFds[i].fd = clientSocket;
                    break;
                }
            }
        }
    }
}

void    Server::receiveRequests()
{
    char buffer[1024];
    for (int i = 1; i < MAX_CLIENTS; i++)
    {
        if (this->pollFds[i].fd != 1 && (this->pollFds[i].revents & POLL_IN))
        {
            //receive requests from clients
            int bytes = recv(this->pollFds[i].fd, buffer, sizeof (buffer), 0);
            buffer[bytes] = '\0';
            std::cout << "Received: from client {"<< i << "}: " << buffer << std::endl;
        }
    }
}

void    Server::sendResponses()
{
    if (this->serverStatus > 0)
    {
        //send responses to clients
        send(this->pollFds[1].fd, "Hello from server, welcome to you HOME\n", 40, 0);
    }
}

void    Server::closefds()
{
    //close all the client sockets and the server socket
    for (int i = 0; i < MAX_CLIENTS + 1; i++)
    {
        close(this->pollFds[i].fd);
    }
}
