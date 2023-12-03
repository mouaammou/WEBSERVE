/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oneServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/02 11:18:53 by mouaammo         ###   ########.fr       */
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
#include <sys/signal.h>

#include <csignal>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include "../includes/Request.hpp"

class oneServer
{
    private:
		struct addrinfo *result;//getaddrinfo() function
        int serverSocket;//server socket: socket() function
        std::string severPort;//server port
        std::vector<int> pollFds;//pollfd structure for server socket and connected clients
		int videoFd;//video file descriptor
		int videoSize;//video file size
		std::string responseVideo;//response video
		int bytesSent;//bytes sent

    public:
		Request ClientRequest;//request object
        oneServer(std::string port);
		~oneServer();

		void    bindServerSocket();//set server socket: create socket, set socket options
		void	setServerSocket();//get server socket: getaddrinfo() function
		void    listenForConnections();//fill pollfd structure for server socket: pollfd structure for server socket
		void    pollEvents();//wait for events: poll() function
		bool    receiveRequests(int &fd);//receive requests from clients: recv() function
		bool    sendResponse(int fd);//send responses to clients: send() function
};
