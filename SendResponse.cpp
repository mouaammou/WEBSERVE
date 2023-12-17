/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 01:14:01 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/17 01:44:27 by samjaabo         ###   ########.fr       */
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

class SendFile
{
	private:

	static std::map<int, SendFile*>	files;
	int		filefd;
	int		sfd;
	off_t	offset;
	off_t	length;

	SendFile( int ffd, int sfd )
	{
		filefd = ffd;
		offset = 0;
		length = 0;
		this->sfd = sfd;
	}

	public:

	bool sendData( void )
	{
		short d = sendfile(filefd, sfd, offset, &length, NULL, 0);
		offset += length;
		length = 0;
		if (d == 0)
			return (close(filefd), true);
		return false;
	}
	
	static void build( int ffd, int sfd )
	{
		files[sfd] = new SendFile(ffd, sfd);
	}

	static void send( int sfd )
	{
		if (files.find(sfd) == files.end())
			return ;
		if (files[sfd]->sendData())
		{
			delete files[sfd];
			files.erase(sfd);
		}
	}
};
std::map<int, SendFile*>	SendFile::files;

class SendHeaders
{
	private:

	static std::map<int, SendHeaders*>	headers;
	std::string	data;
	int			sfd;

	SendHeaders( std::string &data, int sfd )
	{
		this->data = data;
		this->sfd = sfd;
	}

	public:

	bool sendData( void )
	{
		ssize_t d = write(sfd, data.c_str(), data.length());
		if (d == 0 && data.empty())
			return true;
		data.erase(0, d);
		return false;
	}

	static void build( std::string &data, int sfd )
	{
		headers[sfd] = new SendHeaders(data, sfd);
	}

	static bool send( int sfd )
	{
		if (headers.find(sfd) == headers.end())
			return true;
		if (headers[sfd]->sendData())
		{
			delete headers[sfd];
			headers.erase(sfd);
			return true;
		}
		return false;
	}
};
std::map<int, SendHeaders*>	SendHeaders::headers;

class SendResponse
{
	public:

	SendResponse( int sfd )
	{
		//call this from poll loop
		if ( ! SendHeaders::send(sfd))
			return ;
		SendFile::send(sfd);
	}

	SendResponse( std::string &headers, int ffd, int sfd )
	{
		// call this from Response class
		SendHeaders::build(headers, sfd);
		if (ffd != -1)
			SendFile::build(ffd, sfd);
	}
};