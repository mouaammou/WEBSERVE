/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:36 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/11 15:44:28 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <ctime>
#include <sys/time.h>
// #include "Response.cpp"
#include "PipeStream.hpp"
#include "ParseCGIOutput.hpp"
#include "Config.hpp"
#include "../../includes/Request.hpp"
#include "Execute.hpp"

class PipeStream;

class CGI : private Execute
{
	public:

	static std::vector<pid_t>			pids;//private
	static std::map<int, CGI*>			runing_processes;//private

	static void remove( int fd );
	static void checkExitedProcess( void );
	static std::map<int, CGI*>::iterator getProcess( pid_t pid );

	public:

	static void build( config &args );
	static void checkTimeoutAndExitedProcesses( void );

	private:

	static const int64_t	MAX_MSEC_TO_TIMEOUT = 800;
	int64_t					timeout_start;
	bool					one_time_kill;
	
	config					&args;
	PipeStream				*pipe_stream;
	pid_t					pid;

	bool runProcess( void );
	int64_t getTime( void );

	public:

	CGI( config &args);
	~CGI( void );

	bool run(void );
	void timeout( void );
	void onProcessExit( int status = -1 );
	pid_t getPid( void ) const;
	// PipeStream* getPipeStream( void );
};

