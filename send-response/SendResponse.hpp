/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 11:53:25 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/20 19:41:44 by samjaabo         ###   ########.fr       */
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

	bool sendString( void );
	static void build( int ffd, int sfd );
	static bool send( int sfd );
};


class SendString
{
	private:

	static std::map<int, SendString*>	save;
	std::string	data;
	int			sfd;

	SendString( std::string const &data, int sfd );

	public:

	bool sendString( void );
	static void build( std::string const &data, int sfd );
	static bool send( int sfd );
};

class SendResponse
{
	private:

	public:

	static bool send( int sfd );
	SendResponse( std::string const &data, int ffd, int sfd );
};