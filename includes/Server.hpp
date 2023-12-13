/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/13 21:23:05 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdio.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#include <sys/stat.h>

#include <csignal>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[0;36m"

#define PERMISSION_CHECK 0644
#define MAX_REQUEST_SIZE 50000

typedef struct config
{
	std::string location[100];
	std::string root;
	std::string indexfile;
	std::string autoindex;
	std::string cgi;
	std::string cgi_path;
	std::string methods;
}				t_config;

class Request;
#include "../includes/Request.hpp"

class Server
{
	private:
		struct addrinfo *result;//getaddrinfo() function
		int serverSocket;//server socket: socket() function
		std::string severPort;//server port
		std::vector<pollfd> pollFds;//pollfd structure for server socket and connected clients
		//map for clients
		std::map<int, Request*> httpClients;
	
		t_config serverConfigFile;
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
