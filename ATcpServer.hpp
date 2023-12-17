/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ATcpServer.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/11 10:18:20 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/17 23:39:22 by samjaabo         ###   ########.fr       */
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


class ATcpServer
{
	public:
		ATcpServer( void );
		ATcpServer( ATcpServer const &other );
		ATcpServer& operator=( ATcpServer const &other );
		~ATcpServer( void );

		virtual bool	onPollIn(int fd);
		virtual bool	onPollOut(int fd);
		void	onPollAccept(int fd);
		void	pollEventLoop( void );

	private:
		static uint32_t buffsize;
		std::vector<struct pollfd>	pollfds;
		std::map<int, std::string>	buffers;
		std::string			uri;

		void addNewFd( int fd );
		void removeFd( int fd );
		void establishServerSocket(uint16_t port);
		void printServerAddress( int fd );
};
