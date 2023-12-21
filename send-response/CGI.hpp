/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:36 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 01:33:07 by samjaabo         ###   ########.fr       */
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
#include "PipeStream.cpp"
#include "ParseCGIOutput.cpp"


class CGI
{
	public:

	static std::vector<pid_t>			pids;//private
	static std::map<int, CGI*>			runing_processes;//private

	static void checkExitedProcess( void );
	static std::map<int, CGI*>::iterator getProcess( pid_t pid );

	public:

	static void build( t_config &config, int client_fd );
	static void checkTimeoutAndExitedProcesses( void );

	private:

	static const int64_t	MAX_MSEC_TO_TIMEOUT = 5000;
	int64_t					timeout_start;
	bool					one_time_kill;

	const std::string		INTERPRETER;
	t_config				&config;
	PipeStream				*input_pipe;
	PipeStream				*output_pipe;
	pid_t					pid;
	int						client_fd;

	bool runProcess( void );
	int64_t getTime( void );

	public:

	CGI( t_config &config, int fd );
	~CGI( void );

	void run(void );
	void timeout( void );
	void onProcessExit( int status = -1 );
	pid_t getPid( void ) const;
};

