/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 01:14:01 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/18 12:11:27 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SendResponse.hpp"


std::map<int, SendFile*>	SendFile::files;


SendFile::SendFile( int ffd, int sfd )
{
	filefd = ffd;
	offset = 0;
	length = 0;
	this->sfd = sfd;
}

bool SendFile::sendString( void )
{
	short d = sendfile(filefd, sfd, offset, &length, NULL, 0);
	offset += length;
	length = 0;
	if (d == 0)
		return (close(filefd), true);
	return false;
}

void SendFile::build( int ffd, int sfd )
{
	files[sfd] = new SendFile(ffd, sfd);
}

bool SendFile::send( int sfd )
{
	if (files.find(sfd) == files.end())
		return true;
	if (files[sfd]->sendString())
	{
		delete files[sfd];
		files.erase(sfd);
		return true;
	}
	return false;
}

// ---------------------------------------------

std::map<int, SendString*>	SendString::save;

SendString::SendString( std::string const &data, int sfd )
{
	this->data = data;
	this->sfd = sfd;
}

bool SendString::sendString( void )
{
	ssize_t d = write(sfd, data.c_str(), data.length());
	if (d == 0 && data.empty())
		return true;
	data.erase(0, d);
	return false;
}

void SendString::build( std::string const &data, int sfd )
{
	save[sfd] = new SendString(data, sfd);
}

bool SendString::send( int sfd )
{
	if (save.find(sfd) == save.end())
		return true;
	if (save[sfd]->sendString())
	{
		delete save[sfd];
		save.erase(sfd);
		return true;
	}
	return false;
}


//---------------------------------------------


bool SendResponse::send( int sfd )
{
	// call this from poll loop
	if ( ! SendString::send(sfd))
		return false;
	if (SendFile::send(sfd))
		return true;
	return false;
}

SendResponse::SendResponse( std::string const &data, int ffd, int sfd )
{
	// call this from Response class
	// 'ffd = -1' if you don't want to send any file
	SendString::build(data, sfd);
	if (ffd != -1)
		SendFile::build(ffd, sfd);
}

int main()
{
	std::string s = "hello\n";
	int fd = open("test.txt", O_RDONLY);
	SendResponse(s, fd, 1);
	SendResponse::send(1);
	return 0;
}
