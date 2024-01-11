/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PipeStream.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:12 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/11 16:59:29 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PipeStream.hpp"
#include "../include/CGI.hpp"
#include "../../includes/webserv.hpp"
#include "../../includes/Request.hpp"
#include "../../includes/pollServers.hpp"

/*** when new CGI is created a new PipeStream is added here Automaticly ***/
std::map<int, PipeStream*>		 PipeStream::pipes;
/*** END ***/

bool  PipeStream::readFromPipe( void )
{
	/*** true means Error ***/
	int ret = read(fd, buffer, buffer_size);
	if (ret == 0)
		return true;
	if (ret == -1)
	{
		// std::cerr << "status:" << exit_status <<  std::endl;//error remove errno
		exit_status = -1;
		std::cerr << "Error: read() from cgi pipe failed: " << fd << strerror(errno) << std::endl;//error remove errno
		return true;
	}
	buffer[ret] = '\0';
	saved_output += buffer;
	// std::cout << buffer << std::endl;
	return false;
}

void  PipeStream::onReadComplete( void )
{
	// pipe_stream->setExitStatus(status);
	// std::cout << "build cgi >>>>>>>> " << args.request->getFd() << std::endl;
	// std::cout << "build cgi >>>>>>>> " << args.request->getContentType() << std::endl;
	// std::cout << "build cgi >>>>>>>> " << args.Server->getHostName() << std::endl;
	// std::cout << "onProcessExit is called=>" <<  args.request->getFd() << std::endl;
	ParseCGIOutput().response(exit_status, saved_output, conf);
}

bool  PipeStream::isIsPipeStream( pollfd pfd )
{
	/*** use it for pollin ***/
	std::map<int, PipeStream*>::iterator it;
	it = pipes.find(pfd.fd);
	if (it == pipes.end())
		return false;
	bool ret = false;
	if ((pfd.revents & POLLIN))
		ret = it->second->readFromPipe();
	if (ret || pfd.revents & POLLERR || pfd.revents & POLLNVAL)
	{
		CGI::remove(it->second->getConfig().request->getFd());
		// on cgi exit or failure
		it->second->onReadComplete();
		PipeStream::remove(pfd.fd);
		std::cout << "read complete" << std::endl;
	}
	return true;
}

void  PipeStream::remove( int fd )
{
	/*** use it on CGI remove ***/
	std::cout << "PipeStream::remove( int fd )" << std::endl;
	std::map<int, PipeStream*>::iterator it;
	it = pipes.find(fd);
	if (it == pipes.end())
		return ;
	std::cout << "PipeStream::remove( int fd ) 200" << std::endl;
	it->second->getConfig().poll->removeFileDescriptor(fd);
	delete it->second;
	pipes.erase(it);
}

bool  PipeStream::init( void ) //call it in cgi
{
	/*** use it before fork  ***/
	fds[0] = -1;
	fds[1] = -1;
	if (pipe(fds) == -1)
	{
		std::cerr << "Error 0: pipe() failed: " << strerror(errno) << std::endl;
		return false;
	}
	fd = fds[0];
	return true;
}

void  PipeStream::inChildProcess( void ) //call it in child proccess in cgi
{
	/*** WE ARE IN CHILD PROCESS AFTER FORK ***/

	/*** use it to put pipe in stdout ***/
	close(fds[0]);
	if (dup2(fds[1], STDOUT_FILENO) == -1)
	{
		std::cerr << "Error 1: dup2() failed: " << fds[1] << strerror(errno) << std::endl;
		close(fds[1]);
		std::exit(EXIT_FAILURE);
	}
	close(fds[1]);
	/*** output pipe stream end ***/

	/*** put pipe in STDIN, And write request body to it ***/
	if (pipe(fds) == -1)
	{
		std::cerr << "Error: pipe() failed: " << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	if (dup2(fds[0], STDIN_FILENO) == -1)
	{
		std::cerr << "Error 2: dup2() failed: " << strerror(errno) << std::endl;
		close(fds[0]);
		close(fds[1]);
		std::exit(EXIT_FAILURE);
	}
	close(fds[0]);
	if (write(fds[1], conf.request->getRequestBody().c_str(), conf.request->getRequestBody().length()) == -1)
	{
		std::cerr << "Error: while writing request body to cgi stdin" << std::endl;
		close(fds[0]);
		close(fds[1]);
		std::exit(EXIT_FAILURE);
	}
	if (close(fds[1]) == -1)
	{
		std::cerr << "Error: closing write end of pipe in cgi  failed: " << strerror(errno) << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void  PipeStream::inParentProcess( void )//call it in cgi
{
	/*** use it after execve to close uneeded fd
	 * and pollin cgi output ***/
	conf.poll->addFileDescriptor(fd);
	close(fds[1]);
}

PipeStream::PipeStream( t_config &conf ) : conf(conf)
{
	fd = -1;
	buffer_size = 16000;
	buffer = new char[buffer_size];
	exit_status = 0;
	if ( ! init())
		fd = -1;
}

PipeStream::~PipeStream( void )
{
	std::cout << "PipeStream::~PipeStream( void )" << std::endl;
	if (fd != -1)
		close(fd);
	delete [] buffer;
	if (fd != -1)
		conf.poll->removeFileDescriptor(fd);
}

t_config&  PipeStream::getConfig( void )
{
	return conf;
}

void PipeStream::setExitStatus( int status )
{
	exit_status = status;
}

void		PipeStream::setFd( void )
{
	fd = -1;
}
int		PipeStream::getFd( void )
{
	return fd;
}