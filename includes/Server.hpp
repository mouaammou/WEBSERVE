/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/22 20:40:03 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/webserv.hpp"
#include "../includes/Request.hpp"
#include "../includes/Method.hpp"
#include <string>

class Request;
class Method;

class Server
{
	private:
		struct addrinfo *result;//getaddrinfo() function
		int serverSocket;//server socket: socket() function
		std::string severPort;//server port
		//map for clients
		std::string requested_location;
	public:
		std::string request_statuCode;
		Method* pointedMethod;//private, and set a setter
		
		t_config serverConfigFile;
		std::map<int, Request*> httpClients;
		Server(t_config serverConfigFile);
		~Server();

		Method* getPointedMethod() const;
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
		
		std::string getRequestedLocation(std::string path);//get the requested location
		std::string getTranslatedPath(std::string location);//get the translated path
		bool	isClient(int fd);//check if the fd is a client
		void    closefds();//close client sockets: close() function


		void	printf_t_config(t_config config_file);
};
