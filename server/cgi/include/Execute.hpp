/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execute.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 18:31:05 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/24 11:35:38 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../../socket/include/webserv.hpp"
#include "../../Request/includes/Request.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <cctype>

class Execute
{
private:

	std::vector<char *>	env;
	std::vector<char *>	args;
	std::map<std::string, std::string> headers;
	config				&conf;

	void addAllEnvVars( void );
	void addEnvVar( std::string key, std::string value );
	std::string getRemoteAddr( void );
	std::string getServerName( void );
	void addAllArgs( void );
	char *strdup( std::string str );
	std::string toUpperCaseVar( std::string str );
	void requestHeaderstToCGIVariables( void );

public:

	Execute( config &args );
	char**	getEnv( void );
	char**	getArgs( void );
	char* getInterpreterPath( void );
	~Execute( void );
};
