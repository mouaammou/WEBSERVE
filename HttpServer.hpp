/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 10:18:20 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/21 13:43:38 by mouaammo         ###   ########.fr       */
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
	private:
		static uint32_t buffsize;
		std::vector<struct pollfd>	pollfds;

		void addNewFd( int fd );
		void removeFd( int fd );
		void establishServerSocket(uint16_t port);
		void printServerAddress( int fd );

	public:
		HttpServer();
		~HttpServer();

		virtual bool	onPollIn(int fd);
		virtual bool	onPollOut(int fd);
		void	onPollAccept(int fd);
		void	pollEventLoop( void );
};
