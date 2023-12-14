/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/14 18:34:30 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once


#include "../includes/webserv.hpp"
#include <_types/_intmax_t.h>

class Request;
#include "../includes/Request.hpp"

class Server
{
	private:
		struct addrinfo *result;//getaddrinfo() function
		int serverSocket;//server socket: socket() function
		std::string severPort;//server port
		//map for clients
		t_config serverConfigFile;
	
	public:
		std::map<int, Request*> httpClients;
		Server(t_config serverConfigFile);
		~Server();

		int		getServerSocket() const;
		void    bindServerSocket();//set server socket: create socket, set socket options
		void	setServerSocket();//get server socket: getaddrinfo() function
		void    addFileDescriptor(int fd);//add file descriptor to the pollfd array
		void	removeFileDescriptor(int fd);//remove file descriptor from the pollfd array
		int		listenForConnections();//fill pollfd structure for server socket: pollfd structure for server socket
		void    pollEvents();//wait for events: poll() function
		void    acceptConnections();//check for events on server socket: accept() function

		void	addClient(int fd);//add new client to the map
		void	removeClient(int fd);//remove client from the map

		void	handleRequest(int fd);//handle client request

		bool	isClient(int fd);//check if the fd is a client
		void    closefds();//close client sockets: close() function
};
