/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/10 23:24:24 by mouaammo         ###   ########.fr       */
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

typedef struct config
{
	std::string location="/index.html";
	std::string root="/Users/mouaammo/Desktop/WEBSERVE/html";
	std::string translated_path = location + root;
	std::string index;
	std::string autoindex;
	std::string cgi;
	std::string cgi_path;
	std::string cgi_extension;
	std::string cgi_param;
	std::string upload;
	std::string upload_path;
	std::string upload_store;
	std::string upload_size;
	std::string methods;
}				t_config;

class Server
{
	private:
		struct addrinfo *result;//getaddrinfo() function
		int serverSocket;//server socket: socket() function
		std::string severPort;//server port
		std::vector<pollfd> pollFds;//pollfd structure for server socket and connected clients
		//map for clients
		std::map<int, Client*> httpClients;
	
		t_config server_config;
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
