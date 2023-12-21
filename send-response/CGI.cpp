/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:36 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 01:35:33 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "CGI.hpp"

std::map<int, CGI*>			CGI::runing_processes;
std::vector<pid_t>			CGI::pids;


void CGI::checkExitedProcess( void )
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
		// std::cout << Response::getClientResponse(0) << std::endl	; //error replace 0 with client_fd
		// exit(0);// error remove this
	}
}

std::map<int, CGI*>::iterator CGI::getProcess( pid_t pid )
{
	std::map<int, CGI*>::iterator it;
	for (it = runing_processes.begin(); it != runing_processes.end(); ++it)
	{
		if (it->second->getPid() == pid)
			return it;
	}
	return runing_processes.end();
}

void CGI::build( t_config &config, int client_fd )//call this for new cgi
{
	CGI *cgi = new CGI(config, client_fd);
	runing_processes[client_fd] = cgi;
	cgi->run();
	// std::cout << "build cgi " << runing_processes.size() << std::endl;
}

void CGI::checkTimeoutAndExitedProcesses( void ) //use it in poll
{
	// std::cout << "checkTimeoutAndExitedProcesses: " << runing_processes.size() << std::endl;
	std::map<int, CGI*>::iterator it;
	for (it = runing_processes.begin(); it != runing_processes.end(); ++it)
		it->second->timeout();
	checkExitedProcess();
}

bool CGI::runProcess( void )
{
	pid = fork();
	if (pid == -1)
		return false;
	else if (pid == 0)
	{
		if (chdir(config.root.c_str()) != 0)
			std::exit(EXIT_FAILURE);
		input_pipe->inChild();
		output_pipe->inChild();
		// execle();
		// std::cout << "TEST PIPE BEFORE RUNING: " << config.translated_path << std::endl;
		execlp(INTERPRETER.c_str(), INTERPRETER.c_str(), config.translated_path.c_str(), NULL);
		std::cerr << "Error: execlp() failed to exec " << config.translated_path << std::endl;
		std::exit(EXIT_FAILURE);
		return false;
	}
	pids.push_back(pid);
	input_pipe->inParent();
	output_pipe->inParent();
	return true;
}

int64_t CGI::getTime( void )
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
	{
		std::cerr << "Error: gettimeofday() failed: " << strerror(errno) << std::endl;
		exit(0);
		return 0;
	}
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

CGI::CGI( t_config &config, int fd ) : config(config), INTERPRETER("python3")
{
	client_fd = fd;
	input_pipe = new PipeStream(PipeStream::PARENT_WRITE_CHILD_READ);
	output_pipe = new PipeStream(PipeStream::PARENT_READ_CHILD_WRITE);
	pid = -1;
	timeout_start = getTime();
	one_time_kill = false;
}

CGI::~CGI( void )
{
	if (input_pipe)
		delete input_pipe;
	if (output_pipe)
		delete output_pipe;
}

void CGI::run(void )
{
	if (input_pipe->fail() || output_pipe->fail())
		return onProcessExit();
	if ( ! runProcess())
		return onProcessExit();
	timeout_start = getTime();
}

void CGI::timeout( void )
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

void CGI::onProcessExit( int status = -1 )
{
	if (status == -1)
	{
		// pipe or fork error
		Response(config, client_fd, "500");
		return ;
	}
	output_pipe->parentRead();
	std::string output = output_pipe->getSavedOutput();
	// std::cout << "---> "<<output << std::endl;
	ParseCGIOutput().response(status, output, client_fd, config);
}

pid_t CGI::getPid( void ) const
{
	return pid;
}



