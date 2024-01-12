/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:36 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/11 23:22:44 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"

std::map<int, CGI*>			CGI::runing_processes;
std::vector<pid_t>			CGI::pids;

void CGI::remove( int fd )
{
	std::map<int, CGI*>::iterator it;
	it = runing_processes.find(fd);//RUNING CGI SCRIPTS
	if (it == runing_processes.end())
		return ;
	// std::cout << "pid " << pid << " exited" << std::endl;
	pid_t pid = it->second->getPid();
	if (pid != -1)
		kill(pid, SIGKILL);
	delete it->second;
	runing_processes.erase(it);
}

void CGI::checkExitedProcess( void )
{
	// ALWAYS AFTER POLL RETURN CHECK FOR ANY EXUTED PROCESSES
	int status = 0;
	for (size_t i=0; i < pids.size(); ++i)
	{
		// don't remove pid from pids vector to collect its exit status
		pid_t pid = waitpid(pids[i], &status, WNOHANG);
		if (pid == -1 || pid == 0)
			continue ;
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = WTERMSIG(status);
		else if (WIFSTOPPED(status) || WIFCONTINUED(status))
			continue;
		// std::cout << "pid " << pid << std::endl;
		std::map<int, CGI*>::iterator it;
		it = getProcess(pid);
		if (it == runing_processes.end())
		{
			// means remove is called, probaly timeout or read fail in Pipestream or socket closed
			pids.erase(pids.begin() + i);
			continue ;
		}
		// std::cout << "pid " << pid << " exited" << std::endl;
		it->second->onProcessExit(status);
		pids.erase(pids.begin() + i);
		// means cgi exited
	}
}

std::map<int, CGI*>::iterator CGI::getProcess( pid_t pid )
{
	/*** IF REMOVE IS CALLED THERE WILL BE NO CGI OBJECT ASSOCIATED WITH THE PID ***/
	std::map<int, CGI*>::iterator it;
	for (it = runing_processes.begin(); it != runing_processes.end(); ++it)
	{
		if (it->second->getPid() == pid)
			return it;
	}
	return runing_processes.end();
}

void CGI::build( config &args )//call this for new cgi
{
	CGI *cgi = new CGI(args);
	if ( ! cgi->run())
	{
		delete cgi;
		return ;
	}
	runing_processes[args.request->getFd()] = cgi;
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
	/*** false means pipe or fork failure ***/
	if (pipe_stream->getFd() == -1)
		return false;
	pid = fork();
	if (pid == -1)
		return false;
	if (pid == 0)
	{
		pipe_stream->inChildProcess();
		std::cerr << "EXECUTE CGI USING: " <<  args.location.getCgiExe() << "<|" << std::endl;
		// execlp("lsof", "lsof", "-c", "webserv", NULL);

		/*** change current working directroy for relative file access in CGI script ***/
		std::string dir = args.translated_path.substr(0, args.translated_path.find_last_of('/'));
		if (chdir(dir.c_str()) == -1)
		{
			std::cerr << "Error: chdir() failed: " << strerror(errno) << std::endl;
			std::exit(EXIT_FAILURE);
		}
		// std::cerr << "EXECUTE CGI USING: " <<  args.location.getCgiExe() << "<|" << std::endl;
		execve(args.location.getCgiExe().c_str(), getArgs(), getEnv());
		// execve("/Users/samjaabo/Desktop/webserv/cgi_tester", getArgs(), getEnv());
		std::cerr << "Error: execve() failed to exec " << args.translated_path << std::endl;
		std::exit(EXIT_FAILURE);
		return false;
	}
	pids.push_back(pid);
	pipe_stream->inParentProcess();
	// std::ostringstream pwd; pwd << pid;//getpid();
	// system((std::string("lsof -p ") + pwd.str()).c_str());
	// std::cout << "-----------------" << std::endl;
	// system("lsof -c webserv");
	return true;
}

int64_t CGI::getTime( void )
{
	struct timeval tv;
	if (gettimeofday(&tv, NULL))
	{
		//timeout will happen
		// std::cerr << "Error: gettimeofday() failed: " << strerror(errno) << std::endl;
		// exit(0);//error dont exit
		//internal server error when it fails because the timeout will kill it
		return (0);
	}
	return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}


CGI::CGI( config &args) : Execute(args) , args(args)
{
	pid = -1;
	timeout_start = getTime();
	one_time_kill = false;
	args.cgi = false;
	pipe_stream = new PipeStream(args);
	PipeStream::pipes[pipe_stream->getFd()] = pipe_stream;
}

CGI::~CGI( void )
{

}

bool CGI::run(void )
{
	/*** false on error ***/
	if ( ! runProcess())
	{
		args.response_code = "500";
		Response tmp(args);
		return false;
	}
	timeout_start = getTime();
	return true;
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
		args.response_code = "504";
		kill(pid, SIGKILL);
		one_time_kill = true;
	}
}

void CGI::onProcessExit( int status )
{
	pid = -1;
	pipe_stream->setExitStatus(status);
	/*** cgi didnt run so pipefd didnt set to pollin,
	 * so we need to call readFromPipe() manually
	 * to let it generate Response and clear resources  ***/
	// std::cout << "build cgi >>>>>>>> " << args.request->getFd() << std::endl;
	// std::cout << "build cgi >>>>>>>> " << args.request->getContentType() << std::endl;
	// std::cout << "build cgi >>>>>>>> " << args.Server->getHostName() << std::endl;
	// std::cout << "onProcessExit is called=>" <<  args.request->getFd() << std::endl;
}

pid_t CGI::getPid( void ) const
{
	return pid;
}

// PipeStream* CGI::getPipeStream( void )
// {
// 	return pipe_stream;
// }

