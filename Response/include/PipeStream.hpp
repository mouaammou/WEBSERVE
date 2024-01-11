/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PipeStream.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:12 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/11 02:46:15 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <cstdio>
#include <vector>
#include <map>
#include <poll.h>
#include "ParseCGIOutput.hpp"



class PipeStream
{
	private:

	int				fd;
	t_config		&conf;// if read fails close it
	int				buffer_size;
	char			*buffer;
	std::string		saved_output;
	int				fds[2];
	int 			exit_status;


	public:

	static std::map<int, PipeStream*>		pipes;
	static bool isIsPipeStream( pollfd pfd );
	static void remove( int fd );
	
	bool readFromPipe( void );
	bool init( void ); //call it in cgi
	void inChildProcess( void ); //call it in child proccess in cgi
	void inParentProcess( void );//call it in cgi;
	void  onReadComplete( void );

	PipeStream( t_config &conf );
	~PipeStream( void );

	void				setExitStatus( int status );
	t_config&			getConfig( void );
	int			getFd( void );
	void		setFd( void );
};