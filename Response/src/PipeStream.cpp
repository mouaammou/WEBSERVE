/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PipeStream.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:12 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/23 01:14:38 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/PipeStream.hpp"



std::string PipeStream::getSavedOutput( void )
{
	return *saved_output;
}

void PipeStream::parentRead( void ) // true on error or eof
{
	// if (readfd == -2)
	// 	return ;
	char *buffer = new char[buffer_size + 1];
	ssize_t n = ::read(readfd, buffer, buffer_size);
	if (n == -1 || n == 0)
	{
		close(readfd);
		readfd = -1;
		delete [] buffer;
		return ;
	}
	buffer[n] = '\0';
	// std::cout << "read: "<<n <<"|" << buffer << std::endl;
	saved_output->append(buffer);
	delete[] buffer;
}

bool PipeStream::parentWrite( std::string &output ) // true on error
{
	if (writefd == -2)
		return false;
	ssize_t n = ::write(writefd, output.c_str(), output.length());
	if (n == -1)
	{
		close(writefd);
		writefd = -1;
		return true;
	}
	return false;
}

bool PipeStream::fail( void ) const
{
	return (readfd == -1 || writefd == -1);
}

void PipeStream::inParent( void )
{
	if (mode == PARENT_READ_CHILD_WRITE)
	{
		close(writefd);
		writefd = -2;
	}
	else if (mode == PARENT_WRITE_CHILD_READ)
	{
		close(readfd);
		readfd = -2;
	}
}

void PipeStream::inChild( void )
{
	if (mode == PARENT_READ_CHILD_WRITE)
	{
		close(readfd);
		readfd = -2;
		if(dup2(writefd, STDOUT_FILENO) == -1)
			std::exit(10);
		close(writefd);
	}
	else if (mode == PARENT_WRITE_CHILD_READ)
	{
		close(writefd);
		writefd = -2;
		if (dup2(readfd, STDIN_FILENO) == -1)
			std::exit(11);
		close(readfd);
	}
}

PipeStream::PipeStream( int mode )
{
	this->mode = mode;
	readfd = -1;
	writefd = -1;
	int fds[2];
	if (pipe(fds) == -1)
		return ;
	readfd = fds[0];
	writefd = fds[1];
	saved_output = new std::string("");
}

PipeStream::~PipeStream( void )
{
	if (readfd > -1)
		close(readfd);
	if (writefd > -1)
		close(writefd);
	if (saved_output)
		delete saved_output;
}

