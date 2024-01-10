/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGI.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:36 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/09 14:14:20 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CGI.hpp"

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

void CGI::build( config &args )//call this for new cgi
{
	CGI *cgi = new CGI(args);
	runing_processes[args.request->getFd()] = cgi;
	cgi->run();
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
		std::string dir = args.translated_path.substr(0, args.translated_path.find_last_of('/'));
		if (chdir(dir.c_str()) == -1)
		{
			std::cerr << "Error: chdir() failed: " << strerror(errno) << std::endl;
			std::exit(EXIT_FAILURE);
		}
		input_pipe->inChild();
		output_pipe->inChild();
		int fds[2];
		if (pipe(fds))
		{
			std::cerr << "Error: pipe() failed: " << strerror(errno) << std::endl;
			std::exit(EXIT_FAILURE);
		}
		if (dup2(fds[0], STDIN_FILENO) == -1)
		{
			std::cerr << "Error: dup2() failed: " << strerror(errno) << std::endl;
			close(fds[0]);
			close(fds[1]);
			std::exit(EXIT_FAILURE);
		}
		close(fds[0]);
		if (write(fds[1], args.request->getRequestBody().c_str(), args.request->getRequestBody().length()))
		{
			std::cerr << "Error: while writing request body to cgi stdin" << std::endl;
			close(fds[0]);
			close(fds[1]);
			std::exit(EXIT_FAILURE);
		}
		if (close(fds[1]) == -1)
		{
			std::cerr << "Error: closing write end of pipe in cgi  failed: " << strerror(errno) << std::endl;
			std::exit(EXIT_FAILURE);
		}
		// execle();
		// std::cerr << "RUNING CGI: " << args.translated_path.c_str() << std::endl;
		// execlp(INTERPRETER.c_str(), INTERPRETER.c_str(), args.translated_path.c_str(), NULL);
		// // for
		// std::cerr << "@@@@@args.cgi: " <<  args.location.getCgiExe() << std::endl;
		// execve(args.location.getCgiExe().c_str(), getArgs(), getEnv());
		execve("/Users/moouaamm/Desktop/server/cgi_tester", getArgs(), getEnv());
		std::cerr << "Error: execlp() failed to exec " << args.translated_path << std::endl;
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
	input_pipe = new PipeStream(PipeStream::PARENT_WRITE_CHILD_READ);
	output_pipe = new PipeStream(PipeStream::PARENT_READ_CHILD_WRITE);
	pid = -1;
	timeout_start = getTime();
	one_time_kill = false;
	args.cgi = false;
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

void CGI::onProcessExit( int status )
{
	// std::cout << "build cgi >>>>>>>> " << args.request->getFd() << std::endl;
	// std::cout << "build cgi >>>>>>>> " << args.request->getContentType() << std::endl;
	// std::cout << "build cgi >>>>>>>> " << args.Server->getHostName() << std::endl;
	// std::cout << "onProcessExit is called=>" <<  args.request->getFd() << std::endl;
	if (status == -1)
	{
		// pipe or fork error
		// Response(args, client_fd, "500");
		args.response_code = "500";
		Response resp(args);
		return ;
	}
	output_pipe->parentRead();
	std::string output = output_pipe->getSavedOutput();
	std::cout << "CGI OUTPUT---> "<<output << std::endl;
	ParseCGIOutput().response(status, output, args);
}

pid_t CGI::getPid( void ) const
{
	return pid;
}



