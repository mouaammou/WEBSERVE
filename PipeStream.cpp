/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PipeStream.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo < samjaabo@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:12 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/14 22:05:30 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>


class PipeStream
{
	public:

	static const short PARENT_READ_CHILD_WRITE = 2;
	static const short PARENT_WRITE_CHILD_READ = 4;

	private:


	static const size_t buffer_size = 1024;
	int mode;
	int readfd;
	int writefd;

	std::string *saved_output;

	public:

	std::string getSavedOutput( void )
	{
		return *saved_output;
	}

	void parentRead( void ) // true on error or eof
	{
		if (readfd == -2)
			return ;
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

	bool parentWrite( std::string &output ) // true on error
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

	bool fail( void ) const
	{
		return (readfd == -1 || writefd == -1);
	}

	void inParent( void )
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

	void inChild( void )
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

	PipeStream( int mode=0 )
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

	~PipeStream( void )
	{
		if (readfd > -1)
			close(readfd);
		if (writefd > -1)
			close(writefd);
		if (saved_output)
			delete saved_output;
	}
};
