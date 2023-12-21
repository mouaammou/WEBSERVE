/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:36 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/20 23:50:15 by samjaabo         ###   ########.fr       */
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


class Server;
class Location;
class Request;
typedef struct args
{
	std::string		code;
	Server			*server;
	Location		*location;
	Request			*request;
	std::string		translated_path;
	int				sfd;
} t_args;

class CGI
{
	public:

	static std::vector<pid_t>			pids;//private
	static std::map<int, CGI*>			runing_processes;//private
	static void checkExitedProcess( void )
	{
		int status = 0;
		for (size_t i=0; i < pids.size(); ++i)
		{
			pid_t pid = waitpid(pids[i], &status, WNOHANG);
			if (pid == -1 || pid == 0)
				continue ;
			if (WIFEXITED(status))
				status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				status = WTERMSIG(status);
			else if (WIFSTOPPED(status) || WIFCONTINUED(status)) {
				//let it continue and check it later for SIGKILL
				std::cerr << "Process "<< "with pid<" << pid
				<< "> recieves STOP OR CONTINUE Signal" << std::endl;
				continue;
			}
			// std::cout << "pid " << pid << std::endl;
			std::map<int, CGI*>::iterator it;
			it = getProcess(pid);
			if (it == runing_processes.end())
				continue ;
			// std::cout << "pid " << pid << " exited" << std::endl;
			it->second->onProcessExit(status);
			delete it->second;
			runing_processes.erase(it);
			pids.erase(pids.begin() + i);
			// std::cout << Response::getClientResponse(0) << std::endl	; //error replace 0 with args.sfd
			// exit(0);// error remove this
		}
	}
	static std::map<int, CGI*>::iterator getProcess( pid_t pid )
	{
		std::map<int, CGI*>::iterator it;
		for (it = runing_processes.begin(); it != runing_processes.end(); ++it)
		{
			if (it->second->getPid() == pid)
				return it;
		}
		return runing_processes.end();
	}

	public:

	static void build( t_args &args )//call this for new cgi
	{
		CGI *cgi = new CGI(args);
		runing_processes[args.sfd] = cgi;
		cgi->run();
		// std::cout << "build cgi " << runing_processes.size() << std::endl;
	}

	static void checkTimeoutAndExitedProcesses( void ) //use it in poll
	{
		// std::cout << "checkTimeoutAndExitedProcesses: " << runing_processes.size() << std::endl;
		std::map<int, CGI*>::iterator it;
		for (it = runing_processes.begin(); it != runing_processes.end(); ++it)
			it->second->timeout();
		checkExitedProcess();
	}

	private:

	static const int64_t	MAX_MSEC_TO_TIMEOUT = 5000;
	int64_t					timeout_start;
	bool					one_time_kill;

	const std::string		INTERPRETER;
	t_args					&args;
	PipeStream				*input_pipe;
	PipeStream				*output_pipe;
	pid_t					pid;

	bool runProcess( void )
	{
		pid = fork();
		if (pid == -1)
			return false;
		else if (pid == 0)
		{
			if (chdir(args.location->getRoot().c_str()) != 0)
				std::exit(EXIT_FAILURE);
			input_pipe->inChild();
			output_pipe->inChild();
			// execle();
			// std::cout << "TEST PIPE BEFORE RUNING: " << args.translated_path << std::endl;
			execlp(INTERPRETER.c_str(), INTERPRETER.c_str(), args.translated_path.c_str(), NULL);
			std::cerr << "Error: execlp() failed to exec " << args.translated_path << std::endl;
			std::exit(EXIT_FAILURE);
			return false;
		}
		pids.push_back(pid);
		input_pipe->inParent();
		output_pipe->inParent();
		return true;
	}

	int64_t getTime( void )
	{
		struct timeval tv;
		if (gettimeofday(&tv, NULL))
		{
			kill(pid, SIGKILL);
			one_time_kill = true;
			std::cerr << "Error: gettimeofday() failed" << std::endl;
			return 0;
		}
		return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
	}

	public:

	CGI( t_args &args ) : args(args), INTERPRETER("python3")
	{
		input_pipe = new PipeStream(PipeStream::PARENT_WRITE_CHILD_READ);
		output_pipe = new PipeStream(PipeStream::PARENT_READ_CHILD_WRITE);
		pid = -1;
		timeout_start = getTime();
		one_time_kill = false;
	}
	
	~CGI( void )
	{
		if (input_pipe)
			delete input_pipe;
		if (output_pipe)
			delete output_pipe;
	}

	void run(void )
	{
		if (input_pipe->fail() || output_pipe->fail())
			return onProcessExit();
		if ( ! runProcess())
			return onProcessExit();
		timeout_start = getTime();
	}

	void timeout( void )
	{
		if (one_time_kill || pid == -1)
			return ;
		int64_t now = getTime() - timeout_start;
		// std::cout << "timeout_counter: " << now << std::endl;
		// if (now < 0)
		// 	exit(0);
		if (now >= MAX_MSEC_TO_TIMEOUT)
		{
			kill(pid, SIGKILL);
			one_time_kill = true;
		}
	}

	void onProcessExit( int status = -1 )
	{
		if (status == -1)
		{
			// pipe or fork error
			Response(args, args.sfd, "500");
			return ;
		}
		output_pipe->parentRead();
		std::string output = output_pipe->getSavedOutput();
		// std::cout << "---> "<<output << std::endl;
		ParseCGIOutput().response(status, output, args.sfd, args);
	}

	pid_t getPid( void ) const
	{
		return pid;
	}
};

std::map<int, CGI*>			CGI::runing_processes;
std::vector<pid_t>			CGI::pids;

