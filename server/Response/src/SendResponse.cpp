/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SendResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 01:14:01 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/25 23:38:57 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/SendResponse.hpp"

std::map<int, SendFile*>	SendFile::files;

////////////////////////////////////////

SendFile::SendFile( int ffd, int sfd, config &conf) : conf(conf)
{
	offset = 0;
	length = 0;
	
	filefd = ffd;
	this->sfd = sfd;
	READ_SIZE = 100000000;//32KB
	DATA_LIMIT = 100000000;//
	eof = false;
	buffer = new char[READ_SIZE + 1];
	std::memset(buffer, 0, READ_SIZE + 1);
	conf.poll->addFileDescriptor(filefd);
	sizee = 0;
}

void SendFile::removeFileFdFromPoll( void )
{
	conf.poll->removeFileDescriptor(filefd);
	eof = true;
}

void SendFile::readfromFile( void )
{
	if (data.length() >= DATA_LIMIT)
		return ;
	ssize_t d = read(filefd, buffer, READ_SIZE);
	if (d == -1)
	{
		// std::cout << "*** error reading file ***" << std::endl;
		close(sfd);
		return ;
	}
	if (d == 0)
	{
		// std::cout << "*** eof reached ***" << std::endl;
		removeFileFdFromPoll();
		eof = true;
		return ;
	}
	buffer[d] = '\0';
	data.append(buffer, d);
}

bool	SendFile::isSendFileFd( pollfd pfd )
{
	int ssfd = SendFile::getSocketFdWithThisFileFd(pfd.fd);
	if (ssfd == -1)
		return false;
	SendFile *obj = SendFile::files[ssfd];
	if (pfd.revents & (POLLOUT))
		obj->readfromFile();
	else if (pfd.revents & (POLLHUP))
		obj->removeFileFdFromPoll();
	return true;
}

int	SendFile::getSocketFdWithThisFileFd( int ffd )
{
	std::map<int, SendFile*>::iterator it = files.begin();
	for (; it != files.end(); ++it)
	{
		if (it->second->getFileFd() == ffd)
			return it->first;
	}
	return -1;
}

int SendFile::getFileFd()
{
	return filefd;
}

////////////////////////////////////////

// SendFile::SendFile( int ffd, int sfd )
// {
// 	filefd = ffd;
// 	offset = 0;
// 	length = 0;
// 	this->sfd = sfd;
// }

bool SendFile::sendFile( void )
{
	ssize_t d = -2; // nothing sent to do with this value
	if ( ! data.empty())
		d = write(sfd, data.c_str(), data.length());
	if (d == -1)
	{
		// std::cout << "*** error sending file ***" << std::endl;
		close(sfd);
		return (true);
	}
	if (d == 0)
		return (true);
	if (d != -2)
		data.erase(0, d);
	if (d != -2)
		sizee += d;
	if (data.empty() && eof)
		return (true);
	// std::cout << "sizee = " << sizee << "|" << d << std::endl;
	return false;
}

void SendFile::build( int ffd, int sfd, config &conf)
{
	delete files[sfd];
	files[sfd] = new SendFile(ffd, sfd, conf);
}

bool SendFile::send( int sfd )
{
	if (files.find(sfd) == files.end())
		return true;
	if (files[sfd]->sendFile())
	{
		delete files[sfd];
		files.erase(sfd);
		return true;
	}
	return false;
}

//////destructors
SendFile::~SendFile( void )
{
	std::ostringstream oss;
	oss << "temporary/" << sfd << ".output";
	std::remove(oss.str().c_str());
	removeFileFdFromPoll();
	close(filefd);
	delete[] buffer;
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

SendResponse::SendResponse( std::string const &data, int ffd, int sfd, config &conf )
{
	SendString::build(data, sfd);
	if (ffd != -1)
		SendFile::build(ffd, sfd, conf);
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
