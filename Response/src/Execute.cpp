/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execute.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 18:31:05 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/02 01:35:27 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Execute.hpp"

void Execute::addAllEnvVars( void )
{
	// addEnvVar("AUTH_TYPE", "");
	addEnvVar("CONTENT_LENGTH", "0");//body
	addEnvVar("CONTENT_TYPE", "text/html");//if content type in headers
	addEnvVar("GATEWAY_INTERFACE", "CGI/1.1");//vesrion of cgi

	addEnvVar("PATH_INFO", "conf.request->getPathInfo()");
	// addEnvVar("PATH_TRANSLATED", conf.path_info_translated);
	addEnvVar("PATH_TRANSLATED", "conf.path_info_translated");
	
	// addEnvVar("QUERY_STRING", conf.request.getQueryString());
	addEnvVar("QUERY_STRING", "conf.request.getQueryString()");//or "" if no query string

	addEnvVar("REMOTE_ADDR", getRemoteAddr()); // ip of client
	addEnvVar("REMOTE_HOST", ""); //should be passed to cgi
	addEnvVar("REQUEST_METHOD", conf.request->getMethod());
	// addEnvVar("REQUEST_URI", conf.request.getUri());
	addEnvVar("SCRIPT_NAME", conf.translated_path);//script path

	addEnvVar("SERVER_NAME", getServerName());
	addEnvVar("SERVER_PORT", conf.port);
	addEnvVar("SERVER_PROTOCOL", "HTTP/1.1");
	addEnvVar("SERVER_SOFTWARE", "webserv/1.0");

	requestHeaderstToCGIVariables();
	
	env.push_back(NULL);//at the end
}

void Execute::addEnvVar( std::string key, std::string value )
{
	std::string var = key + "=" + value;
	env.push_back(strdup(var));
}

std::string Execute::getRemoteAddr( void )
{
	sockaddr_in	addr;
	socklen_t len = sizeof(addr);
	if (getpeername(conf.request->getFd(), (sockaddr *)&addr, &len) == -1)
		return "";//error
	return inet_ntoa(addr.sin_addr);
}

std::string Execute::getServerName( void )
{
	sockaddr_in	addr;
	socklen_t len = sizeof(addr);
	if (getsockname(conf.server_fd, (sockaddr *)&addr, &len) == -1)
		return "";//error
	return inet_ntoa(addr.sin_addr);
}

void Execute::addAllArgs( void )
{
	std::cout << "conf.location.getCgiExe(): " << conf.location.getName() << std::endl;
	std::cout << "conf.location.getCgiExe(): " << conf.location.getCgiExe() << std::endl;
	args.push_back(const_cast<char*>(conf.location.getCgiExe().c_str()));
	args.push_back(const_cast<char*>(conf.translated_path.c_str()));
	args.push_back(NULL);
}

char* Execute::strdup( std::string str )
{
	char *var = new char[str.length() + 1];
	std::strncpy(var, str.c_str(), str.length());
	var[str.length()] = '\0';
	return var;
}

std::string Execute::toUpperCaseVar( std::string str )
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '-')
			str[i] = '_';
		else
			str[i] = std::toupper(str[i]);
	}
	if (str[str.length() - 1] == ':')
		str.erase(str.length() - 1, 1);
	return std::string("HTTP_") + str;
}

void Execute::requestHeaderstToCGIVariables( void )
{
	std::map<std::string, std::string> &hdrs = conf.request->getRequestHeaders();
	std::map<std::string, std::string>::iterator it;
	for (it = hdrs.begin(); it != hdrs.end() ; it++)
	{
		std::string key = toUpperCaseVar(it->first);
		// printf("key: %s\n", key.c_str());
		if (key == "HTTP_CONTENT_LENGTH" || key == "HTTP_CONTENT_TYPE" || key == "HTTP_TRANSFER_ENCODING" || key == "HTTP_CONNECTION" || key == "HTTP_COOKIE")
			continue;
		headers[key] = it->second;
	}
	for (it = headers.begin(); it != headers.end(); it++)
		addEnvVar(it->first, it->second);
}

Execute::Execute( config &args ) : conf(args)
{
	addAllEnvVars();
	addAllArgs();
}
char**	Execute::getEnv( void )
{
	return env.data();
}
char**	Execute::getArgs( void )
{
	return args.data();
}
char* Execute::getInterpreterPath( void )
{
	return args[0];
}
Execute::~Execute( void )
{
	for (size_t i = 0; i < env.size(); i++)
		delete [] env[i];
	env.clear();
	args.clear();
}

