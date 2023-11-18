/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:56:03 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/18 17:04:42 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"

Server::Server(std::string &port)//
{
    this->severPort = port;
    this->serverSocket = -1;
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
    
    //INITIALIZE POLLFDS
    for (int i = 1; i < MAX_CLIENTS + 1; i++)
    {
        this->pollFds[i].fd = -1;
        this->pollFds[i].events = POLL_IN;
    }
}
