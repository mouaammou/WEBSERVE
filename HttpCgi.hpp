/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCgi.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:45:07 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 22:47:35 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <map>
# include <vector>
# include <cstdlib>
# include <cstring>
# include <cctype>
# include <unistd.h>
# include <fcntl.h>
#include <cstdlib>
#include <csignal>
#include <sstream>
#include <limits.h>
#include "Response/OutputParser.hpp"

class HttpCgi
{
	private:
		static std::map<int, HttpCgi*>	runing;
		static std::string				interpreter;
		// set confi file here one time
	public:
		static bool	fdIsForCgi( int fd );
		static bool	createCgi( std::string request_file );

		static const int INCOMPLETE_BODY = 1;
		static const int SYSCALL_FAILURE = 2;
		static const int TIMED_OUT = 3;
		static const int EXIT_FAIL = 4;
		static const int SUCCESS = 0;

	private:

		std::vector<std::string>	env;
		std::vector<const char *>	execve_env;
		std::vector<const char *>	args;
		pid_t						pid;
		size_t						content_length;
		bool 						evaluated;//for geting content-length one time
		std::string					*buff;//buffer for read (output of cgi)

		bool	createPipeForRequestBody();
		std::vector<std::string> splitHeaders(const std::string& s, char delimiter);
	public:
		std::string					request_body;//buffer for read (output of cgi)
		int							error;
		std::string					buffer;//output
		// int							inputfd;//give this to poll
		int							outputfd;//give this to poll
		HttpCgi( void );
		// HttpCgi( HttpCgi const &other );
		// HttpCgi& operator=( HttpCgi const &other );
		// ~HttpCgi( void );

		bool	run( std::string script );
		void	addEnv(std::string str);
		void	fill_env( void );
		bool	readOutOfCgi( void );
		void	onError( void );

		void	handleFinishedOutput( void );
		std::string	getData( void );
};
