/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 11:53:25 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/25 19:57:04 by samjaabo         ###   ########.fr       */
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
#include "../../socket/include/pollServers.hpp"
# include "../../cgi/include/NewCGI.hpp"

class SendFile
{
	private:

	static std::map<int, SendFile*>	files;
	int		filefd;
	int		sfd;
	off_t	offset;
	off_t	length;
	
	size_t		READ_SIZE; //read buffer size
	size_t		DATA_LIMIT;//if read reads this limit wait until data increased
	bool		eof;//if file reached eof -> true
	std::string data;//file data
	config		&conf;
	char		*buffer;
	int sizee;

	SendFile( int ffd, int sfd );
	SendFile( int ffd, int sfd, config &conf);

	public:

	static void remove( int fd );
	bool sendFile( void );
	static void build( int ffd, int sfd, config &conf);
	static bool send( int sfd );

	~SendFile( void );
	
	static bool isSendFileFd( pollfd pfd );
	static int getSocketFdWithThisFileFd( int ffd );
	void removeFileFdFromPoll( void  );
	void readfromFile( void );
	
	int getFileFd();
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
	SendResponse( std::string const &data, int ffd, int sfd, config &conf );
};