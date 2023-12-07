/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 14:57:33 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/07 01:27:31 by mouaammo         ###   ########.fr       */
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


typedef struct srequest_values
{
	int	*fd;
	bool flagSend;
	int sendBytes;
	int recvBytes;
	std::string responseHeader;
}rqt_values;

class Server
{
    private:
		struct addrinfo *result;//getaddrinfo() function
        int serverSocket;//server socket: socket() function
        std::string severPort;//server port
        std::vector<pollfd> pollFds;//pollfd structure for server socket and connected clients
		int videoFd;//video file descriptor
		int videoSize;//video file size
		bool flagSend;
		int sendBytes;
		// int recvBytes;
		std::string responseHeader;//response header
		Request ClientRequest;//request object
		rqt_values requestValues;//request values
		std::vector<rqt_values> requestValuesVector;//request values vector
    public:
        Server(std::string port);
		~Server();

		void    bindServerSocket();//set server socket: create socket, set socket options
		void	setServerSocket();//get server socket: getaddrinfo() function
		void    addFileDescriptor(int fd);//add file descriptor to the pollfd array
		void	removeFileDescriptor(int &fd);//remove file descriptor from the pollfd array
		void    listenForConnections();//fill pollfd structure for server socket: pollfd structure for server socket
		void    pollEvents();//wait for events: poll() function
		void    acceptConnections();//check for events on server socket: accept() function
		bool    receiveRequests(struct pollfd &fd, int index);//receive requests from clients: recv() function

		bool    sendResponse(struct pollfd &clientFd);//send responses to clients: send() function
		double	bitToMegaBit(double bytes);

		void    closefds();//close client sockets: close() function

		int		get_file_size(int fd);
		void	resetVideoState();
};
