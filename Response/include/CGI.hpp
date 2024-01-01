/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:36 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/01 06:14:38 by samjaabo         ###   ########.fr       */
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


class CGI : private Execute
{
	public:

	static std::vector<pid_t>			pids;//private
	static std::map<int, CGI*>			runing_processes;//private

	static void checkExitedProcess( void );
	static std::map<int, CGI*>::iterator getProcess( pid_t pid );

	public:

	static void build( config &args );
	static void checkTimeoutAndExitedProcesses( void );

	private:

	static const int64_t	MAX_MSEC_TO_TIMEOUT = 800;
	int64_t					timeout_start;
	bool					one_time_kill;

	const std::string		INTERPRETER;
	config					&args;
	PipeStream				*input_pipe;
	PipeStream				*output_pipe;
	pid_t					pid;

	bool runProcess( void );
	int64_t getTime( void );

	public:

	CGI( config &args);
	~CGI( void );

	void run(void );
	void timeout( void );
	void onProcessExit( int status = -1 );
	pid_t getPid( void ) const;
};

