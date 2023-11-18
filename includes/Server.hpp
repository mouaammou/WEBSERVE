/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/18 17:56:55 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <sys/fcntl.h>

#include <iostream>
#include <vector>
#include <string>

#define MAX_CLIENTS 10

class Server
{
    private:
        int serverSocket;
        std::string severPort;
        std::vector<pollfd> pollFds;//pollfd structure for server socket and connected clients
        int serverStatus;//return value of poll() function

    public:
        Server(std::string &port);
        ~Server();

        void    startServer();//set server socket: create socket, set socket options
        void    fillPollFds();//fill pollfd structure for server socket: pollfd structure for server socket
        void    pollEvents();//wait for events: poll() function
        void    acceptConnections();//check for events on server socket: accept() function
        void    receiveRequests();//receive requests from clients: recv() function
        void    sendResponses();//send responses to clients: send() function
        void    closefds();//close client sockets: close() function
};
