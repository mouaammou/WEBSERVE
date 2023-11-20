/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:56:03 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/20 14:00:44 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Server.hpp"
#include <cstdio>

Server::Server(std::string port)//
{
    this->severPort = port;
    this->serverSocket = -1;
    this->serverStatus = -1;
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
    // this->pollFds.push_back((struct pollfd){this->serverSocket, POLLIN, 0});//add server socket to pollfd array
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
    if (listen(this->serverSocket, 5) == -1)//+1 for server socket
    {
        perror("listen");
        exit (EXIT_FAILURE);
    }
    //ADD SERVER SOCKET TO POLLFDS
    struct pollfd server;
    server.fd = this->serverSocket;
    server.events = POLL_IN;//for reading
    this->pollFds.push_back(server);
    std::cout << "Server is listening on port " << this->severPort << std::endl;
    this->keepRunning = true;
    //INITIALIZE POLLFDS
}

void   Server::pollEvents()
{
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
            std::cout << "No events for 5 seconds" << std::endl;
        else
        {
            this->serverStatus = serverStatus;
            if (this->pollFds[0].revents & POLL_IN)//check for events on server socket
            {
                this->acceptConnections();
            }
            else {
                std::cout << "No events on server socket" << std::endl;
            }
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

    if ((clientSocket = accept(this->serverSocket, &clientAddr, &clientAddrLen)) == -1)
    {
        perror("accept");
        return;
    }
    //add the new client socket to the pollfd array
    struct pollfd newClient;
    newClient.fd = clientSocket;
    newClient.events = POLL_IN;
    fcntl(clientSocket, F_SETFL, O_NONBLOCK);
    this->pollFds.push_back(newClient);
}

void    Server::receiveRequests()
{
    char buffer[1024];
    for (size_t i = 1; i < this->pollFds.size(); i++)
    {
        if (this->pollFds[i].revents & POLL_IN)
        {
            //receive requests from clients
            int bytes = recv(this->pollFds[i].fd, buffer, sizeof (buffer), 0);
            buffer[bytes] = '\0';
            if (bytes > 0)
                std::cout << "Received: from client {"<< i << "}: " << buffer << std::endl;
            else if (bytes == -1)
            {
                perror("recv");
                exit(1);
            }
            this->pollFds[i].events = POLLOUT;
        }
    }
    
}

void    Server::sendResponses()
{
    for (size_t i = 1; i < this->pollFds.size(); i++)
    {
        if ((this->pollFds[i].revents & POLLOUT))
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
            send(this->pollFds[i].fd, response.c_str(), response.length(), 0);
            this->pollFds[i].events = POLL_IN;
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
