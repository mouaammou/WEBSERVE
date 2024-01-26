/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 11:53:25 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/26 12:03:43 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <string>
# include <fstream>
# include <map>
# include <vector>
# include <stdexcept>
# include <sstream>
# include <unistd.h>
# include <ctime>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/uio.h>
# include <fcntl.h>
# include <sys/stat.h>
# include "../../cgi/include/NewCGI.hpp"

class SendFile
{
	private:

	static std::map<int, SendFile*>	files;
	int		filefd;
	int		sfd;
	off_t	offset;
	off_t	length;

	SendFile( int ffd, int sfd );

	public:

	static void remove( int fd );
	bool sendString( void );
	static void build( int ffd, int sfd );
	static bool send( int sfd );

	~SendFile( void );
};


class SendString
{
	private:

	static std::map<int, SendString*>	save;
	std::string	data;
	int			sfd;

	SendString( std::string const &data, int sfd );

	public:

	static void remove( int fd );
	bool sendString( void );
	static void build( std::string const &data, int sfd );
	static bool send( int sfd );
};

class SendResponse
{
	private:

	public:

	static void remove( int fd );
	static bool send( int sfd );
	SendResponse( std::string const &data, int ffd, int sfd );
};