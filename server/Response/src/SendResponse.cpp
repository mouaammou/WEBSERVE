/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 11:53:25 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/28 11:57:43 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/SendResponse.hpp"

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
		return (true);
	return false;
}

void SendFile::build( int ffd, int sfd )
{
	delete files[sfd];
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

SendFile::~SendFile( void )
{
	std::ostringstream oss;
	oss << "temporary/" << sfd << ".output";
	std::remove(oss.str().c_str());
	close(filefd);
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
	if (d == -1)
	{
		close(sfd);
		return true;
	}
	if (d == 0)
		return true;
	data.erase(0, d);
	return false;
}

void SendString::build( std::string const &data, int sfd )
{
	delete save[sfd];
	save[sfd] = new SendString(data, sfd);
}

bool SendString::send( int sfd )
{
	// means cgi is running and we should wait for it
	if (NewCGI::active_procs.find(sfd) != NewCGI::active_procs.end())
		return false;
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
	if ( ! SendString::send(sfd))
		return false;
	if (SendFile::send(sfd))
		return true;
	return false;
}

SendResponse::SendResponse( std::string const &data, int ffd, int sfd )
{
	SendString::build(data, sfd);
	if (ffd != -1)
		SendFile::build(ffd, sfd);
}

void SendResponse::remove( int fd )
{
	SendString::remove(fd);
	SendFile::remove(fd);
	std::ostringstream oss;
	oss << "temporary/" << fd << ".output";
	std::remove(oss.str().c_str());
}

void SendString::remove( int fd )
{
	if (save.find(fd) == save.end())
		return ;
	delete save[fd];
	save.erase(fd);
}

void SendFile::remove( int fd )
{
	if (files.find(fd) == files.end())
		return ;
	delete files[fd];
	files.erase(fd);
}
