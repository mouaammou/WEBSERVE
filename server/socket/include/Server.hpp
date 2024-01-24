/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/24 11:33:48 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"
#include "../../Request/includes/Request.hpp"
#include "../../Request/includes/Method.hpp"

class Request;
class Method;

class Server
{
	private:
		int serverSocket;//server socket: socket() function fd
		struct addrinfo *result;//getaddrinfo() function
		std::string severPort;//server port
		//map for clients
		std::string requested_location;

	public:
		Server(t_config config);
		Method* pointedMethod;//private, and set a setter
		t_config serverConfigFile;
		std::map<int, Request*> httpClients;
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

		void	setStatusCode(std::string status_code);//set status code
		std::string	getStatusCode();//get status code

		void	addClient(int fd);//add new client to the map
		void	removeClient(int fd);//remove client from the map
		
		std::string getRequestedLocation(std::string path);//get the requested location
		std::string getTranslatedPath(std::string location, std::string path);//get the translated path
		bool	isClient(int fd);//check if the fd is a client

		void	setConfiguration(t_config configfile);
		void	printf_t_config(t_config config_file);
};
