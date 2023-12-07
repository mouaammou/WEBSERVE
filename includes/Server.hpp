/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/07 23:07:28 by mouaammo         ###   ########.fr       */
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

#include "../includes/Client.hpp"

class Server
{
    private:
		struct addrinfo *result;//getaddrinfo() function
        int serverSocket;//server socket: socket() function
        std::string severPort;//server port
        std::vector<pollfd> pollFds;//pollfd structure for server socket and connected clients
		//map for clients
		std::map<int, Client*> httpClients;
	
    public:
        Server(std::string port);
		~Server();

		void    bindServerSocket();//set server socket: create socket, set socket options
		void	setServerSocket();//get server socket: getaddrinfo() function
		void    addFileDescriptor(int fd);//add file descriptor to the pollfd array
		void	removeFileDescriptor(int fd);//remove file descriptor from the pollfd array
		void    listenForConnections();//fill pollfd structure for server socket: pollfd structure for server socket
		void    pollEvents();//wait for events: poll() function
		void    acceptConnections();//check for events on server socket: accept() function

		double	bitToMegaBit(double bytes);

		void	addNewClient(int fd);//add new client to the map
		void	removeClient(int fd);//remove client from the map

		void    closefds();//close client sockets: close() function
};
