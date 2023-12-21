/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PipeStream.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:12 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 21:25:59 by samjaabo         ###   ########.fr       */
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

	std::string getSavedOutput( void );
	void parentRead( void );
	bool parentWrite( std::string &output );
	bool fail( void ) const;
	void inParent( void );
	void inChild( void );

	PipeStream( int mode=0 );
	~PipeStream( void );
};
