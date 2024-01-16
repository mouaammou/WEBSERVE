/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 01:14:01 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/16 12:16:15 by samjaabo         ###   ########.fr       */
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
	// std::cout << "==x==>" << write(sfd, 0, 0) << std::endl;
	ssize_t d = write(sfd, data.c_str(), data.length());
	if (d == 0 && data.empty())
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
	{
		// std::cerr << "Wait CGI RESPONSE    " <<  sfd << std::endl;
		return false;
	}
	// in case of cgi is running, we need to wait for it
	if (save.find(sfd) == save.end())
	{
		// std::cerr << "NOTHI9NG	: " << sfd << std::endl;
		return true;
	}
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
	(void)sfd;
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
	// std::cout << "\n\n\nHeaders->\n\n\n" << data << std::endl;
	SendString::build(data, sfd);
	if (ffd != -1)
		SendFile::build(ffd, sfd);
}

void SendResponse::remove( int fd )
{
	// std::cout << "remove send response" << std::endl;
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