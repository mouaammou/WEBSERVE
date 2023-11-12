/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 10:18:20 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 23:07:26 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <vector>
# include <map>
# include <cstring>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/socket.h>
# include <poll.h>

class HttpServer
{
	public:
		HttpServer( void );
		HttpServer( HttpServer const &other );
		HttpServer& operator=( HttpServer const &other );
		~HttpServer( void );

		virtual bool	onPollIn(int fd);
		virtual bool	onPollOut(int fd);
		void	onPollAccept(int fd);
		void	pollEventLoop( void );

	private:
		static uint32_t buffsize;
		std::vector<struct pollfd>	pollfds;
		std::map<int, std::string>	buffers;

		void addNewFd( int fd );
		void removeFd( int fd );
		void establishServerSocket(uint16_t port);
		void printServerAddress( int fd );
};
