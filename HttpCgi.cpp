/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCgi.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:45:10 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 23:06:28 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpCgi.hpp"

std::map<int,	HttpCgi*>	HttpCgi::runing;
std::string					HttpCgi::interpreter = "/usr/bin/python3";

HttpCgi::HttpCgi( void )
{
	buff = new std::string(34000, '\0');
	content_length = 0;
}

// HttpCgi::HttpCgi( HttpCgi const &other )
// {
// 	content_length = 1024;
// 	evaluated = false;
// 	(void) other;
// }

// HttpCgi& HttpCgi::operator=( HttpCgi const &other )
// {
// 	(void)other;
// 	return *this;
// }

// HttpCgi::~HttpCgi( void )
// {
// }

bool HttpCgi::run( std::string script ) // true on success
{
	int fds[2];// for output
	if (pipe(fds) == -1)
	{
		error = SYSCALL_FAILURE;
		return false;
	}
	pid_t id = fork();
	if (id == -1)
	{
		close(fds[0]);
		close(fds[1]);
		error = SYSCALL_FAILURE;
		return false;
	}
	else if (id == 0)
	{
		int bodyfds[2];
		if (pipe(bodyfds) == -1)//only if method==POST
		{
			close(fds[0]);
			close(fds[1]);
			std::exit(EXIT_FAILURE);
		}
		// const char arr[]= {"/"};
		if (write(bodyfds[1], request_body.c_str(), request_body.length()) == -1)
		{
			close(fds[0]);
			close(fds[1]);
			close(bodyfds[0]);
			close(bodyfds[1]);
			std::exit(EXIT_FAILURE);
		}
		close(fds[0]); // close read end of output
		close(bodyfds[1]); // close write end of input
		if (dup2(fds[1], STDOUT_FILENO) == -1 || dup2(bodyfds[0], STDIN_FILENO) == -1)
		{
			close(fds[1]);
			close(bodyfds[0]);
			std::exit(EXIT_FAILURE);
		}
		close(fds[1]);
		close(bodyfds[0]);
		execve_env.push_back(NULL);
		args.push_back(interpreter.c_str());
		args.push_back(script.c_str());
		args.push_back(NULL);
		execve(args.front(), const_cast<char*const*>(args.data()), const_cast<char*const*>(execve_env.data()));
		close(STDOUT_FILENO);
		close(STDIN_FILENO);
		std::exit(EXIT_FAILURE);
		return false;//ture if execve fails
	}
	env.clear();
	execve_env.clear();
	args.clear();
	close(fds[1]); // close write end
	pid = id;
	outputfd = fds[0]; // from cgi
	if (fcntl(outputfd, F_SETFL, O_NONBLOCK) == -1)
	{
		// int status;
		close(outputfd);
		// close(inputfd);
		kill(pid, SIGTERM);
		error = SYSCALL_FAILURE;
		// waitpid(pid, &sta?) ignore exit status
		return false;
	}
	//  use poll to read from  and to pass to pipe
	// std::setenv
	error = SUCCESS;
	return true ;
}

//=============================================

bool HttpCgi::readOutOfCgi( void )
{
	char *str = const_cast<char*>(buff->c_str());
	int n = read(outputfd, str, 34000);
	if (n == -1)
	{
		error = SYSCALL_FAILURE; // an error happened while reading output
		return true;
	}
	str[n] = 0;
	buffer.append(str);
	std::size_t pos = buffer.find("\n\n");
	error = SUCCESS;
	if (content_length == 0)
	{
		// n = read(outputfd, str, 34000);
		// std::cout << "n=" << n << std::endl;
		if (n == 0) //check if the process exited with 0
		{
			int status = 0;
			int id = waitpid(pid, &status, WNOHANG);
			if (id == pid && status != 0)
				error = EXIT_FAIL;
			return true;
		}
		if (pos != std::string::npos && OutputParser::thereIsContentLength(buffer.substr(0, pos + 1)))
		{
			content_length = OutputParser::getContentLength(buffer.substr(0, pos + 1));
			std::size_t body_len = buffer.length() - (pos + 2);
			std::cout << "--bodylen--->" << body_len << std::endl;
			if ( body_len >= content_length ) // body is greater than content-length
			{
				buffer = buffer.substr(0, (pos + 2) + content_length);
				return true;
			}
		}
		return false;
	}
	// exec bottom if content-length is provided
	std::size_t body_len = buffer.length() - (pos + 2);
	if ( body_len >= content_length ) // body is greater than content-length
	{
		buffer = buffer.substr(0, (pos + 2) + content_length);
		return true;
	}
	if (n == 0)
	{
		// if EOF is reached but we did not recieve the full body [only if content-length is provided]
		error = INCOMPLETE_BODY;//proccess exited and body didnt complete or anything else
		return true;
	}
	return false;
}

void	HttpCgi::onError( void )
{
	// delete both fds from poll arrray first
	close(outputfd);
	kill(pid, SIGTERM);
	int status;
	waitpid(pid, &status, WNOHANG);
	// internal server error response
}

// =================================

bool HttpCgi::fdIsForCgi( int fd )
{
	if (runing.find(fd) == runing.end())
		return false ;
	runing[fd]->handleFinishedOutput();
	return true;
}

void	HttpCgi::handleFinishedOutput( void )
{
	bool ret = readOutOfCgi();
	if ( ret && error != SUCCESS )
	{
		//FAILED CGI SCRIPT
		if (error != EXIT_FAIL)
			onError();
		// removeFromPoll(outputfd);
		//Reponse Internal server error
	}
	else if ( ret )
	{
		//SUCCESS CGI SCRIPT
		// removeFromPoll(outputfd);
		close(outputfd);
		kill(pid, SIGTERM);
		int status;
		waitpid(pid, &status, WNOHANG);
		// body is recieved
		// Response success  200
	}
}

bool HttpCgi::createCgi( std::string request_file )
{
	// size_t pos = request_file.find(extention);000000000



	// if (pos == std::string::npos)
	// 	return false;
	// if (request_file.length() - 4 != pos)
	// 	return false;
	// TO DO
	return true;
}

// void HttpCgi::addEnv(std::string str)
// {
// 	env.push_back(str);
// 	execve_env.push_back(env.back().c_str());
// }

// void	HttpCgi::fill_env( void )
// {
// 	std::string arr[] = { //example
// 		"CONTENT_TYPE=",
// 		"CONTENT_LENGTH=",
// 		// "HTTP_USER_AGENT=",
// 		"PATH_INFO=/cgi-bin/myscript",
// 		"QUERY_STRING=name=John&age=25",
// 		"REMOTE_AADR=192.168.1.101",// FOR AUTHENTICATION
// 		"REMOTE_PORT=54652",
// 		// "REMOTE_HOST=",// FOR AUTHENTICATION
// 		"REQUEST_METHOD=GET",// GET AND POST
// 		// "SCRIPT_FILENAME=",// FULL PATH TO CGI SCRIPT
// 		"SCRIPT_NAME=/cgi-bin/myscript", //NAME OF THE SCRIPT
// 		"SERVER_NAME=1337.ma", //HOSTNAME OR IP
// 		"SERVER_SOFTWARE=Apache/2.4.41 (Unix)",
// 		"SERVER_PROTOCOL=HTTP/1.1",
// 	};
// 	for (int i=0; i<11; ++i)
// 	{
// 		addEnv(arr[i]);
// 	}
// }


// bool	HttpCgi::isContentLengthReached( size_t n )
// {
// 	if ( n == 0 || content_length == 0)
// 		return true;
// 	if ( evaluated )
// 	{
// 		content_length -= n;
// 		if (content_length == 0)
// 			return true;
// 		std::cout << "size=" << content_length << std::endl;
// 		return false;
// 	}
// 	size_t pos = buffer.find("\r\n\r\n");
// 	if (pos == std::string::npos)
// 		return false;
// 	// std::cout <<"$"<< buffer.substr(pos+4)<<"$";
// 	evaluated = true;
// 	std::string h = buffer.substr(0, pos).append("\n");
// 	size_t d = h.find("Content-length:");
// 	if (d == std::string::npos)
// 		d = h.find("Content-length ");
// 	if (d == std::string::npos)
// 		return false;
// 	std::string num = h.substr(15, h.find('\n', 15));
// 	// h.clear();
// 	size_t i = 0;
// 	for (; num[i] == ' '; ++i);
// 	for (; std::isdigit(num[i]) ; ++i);
// 	for (; num[i] == ' '; ++i);
// 	if (i != num.length() - 1)
// 	{
// 		// invalid length returned from cgi
// 		// std::cout << i << "&&&\n" << h << std::endl;
// 		close(outputfd);
// 		close(inputfd);
// 		kill(pid, SIGTERM);
// 		return false;
// 	}
// 	content_length = static_cast<size_t>(std::atoll(num.c_str()));
// 	if (content_length == 0)
// 	{
// 		buffer = buffer.substr(0, pos + 4);
// 		return true;
// 	}
// 	size_t body_size = buffer.length() - (pos + 4);
// 	if (body_size >= content_length)
// 	{
// 		buffer = buffer.substr(0, pos + 4 + body_size);
// 		content_length = 0;
// 		return true;
// 	}
// 	content_length -= body_size;
// 	// std::cout << "content_length="<<content_length << "$\npos="<<pos + 4<<"$\nbodysize="<<body_size<<"$\nlength="<<buffer.length()<<"$\n" << h << std::endl;
// 	// buffer = buffer.substr(pos + 4);
// 	return false;
// }

std::string	HttpCgi::getData( void )
{
	return buffer;
}

//====================

// bool	HttpCgi::createPipeForRequestBody( void )
// {
// 	int fd[2];
// 	pipe(fd);

// }


// bool	HttpCgi::isValidHeader( void )
// {
// 	size_t pos = buffer.find("\r\n\r\n");
// 	if (pos == std::string::npos)
// 		return false;
// }

// std::vector<std::string> HttpCgi::splitHeaders(const std::string& s)
// {
// 	std::vector<std::string>	tokens;
// 	std::stringstream			stream(s);
// 	std::string					token;
// 	while (std::getline(stream, token, '\n'))
// 		tokens.push_back(token);
// 	std::map<std::string, std::string> headers;
// 	for (int i=0; i < tokens.size(); ++i)
// 	{
// 		stream.clear();
// 		stream << tokens[i];
// 		while (std::getline(stream, token, ':'))
// 			headers[token] = stream.str();
// 	}
// 	for (int i=0; i < tokens.size(); ++i)
// 	{
// 		stream.clear();
// 		stream << tokens[i];
// 		while (std::getline(stream, token, ':'))
// 			headers[token] = stream.str();
// 	}
// 	std::map<std::string, std::string>::iterator it;
// 	for (it = headers.begin(); it != headers.end(); ++it) {
// 		std::cout << "Key: " << it->first << ", Value: " << it->second << std::endl;
// 	}
// 	return tokens;
// }
