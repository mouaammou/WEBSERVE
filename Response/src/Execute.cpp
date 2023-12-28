/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Execute.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/25 18:31:05 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/28 05:28:48 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "../../includes/Request.hpp"
#include "../../includes/webserv.hpp"
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Execute
{
private:

	std::vector<char *>	env;
	std::vector<char *>	args;
	config				&conf;
	bool				error;
	
public:
	Execute( config &args ) : conf(args)
	{
		error = false;
	}
	
	void addAllEnvVars( void )
	{
		// addEnvVar("AUTH_TYPE", "");
		addEnvVar("CONTENT_LENGTH", "0");//body
		addEnvVar("CONTENT_TYPE", "text/html");//if content type in headers
		addEnvVar("GATEWAY_INTERFACE", "CGI/1.1");//vesrion of cgi

		addEnvVar("PATH_INFO", conf.request->getPath());
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
	}

	void addEnvVar( std::string key, std::string value )
	{
		std::string str = key + "=" + value;
		char *var = new char[str.length() + 1];
		std::strncpy(var, str.c_str(), str.length());
		var[str.length()] = '\0';
		env.push_back(var);
	}

	std::string getRemoteAddr( void )
	{
		sockaddr_in	addr;
		socklen_t len = sizeof(addr);
		if (getpeername(conf.request->getFd(), (sockaddr *)&addr, &len) == -1)
			return "";//error
		return inet_ntoa(addr.sin_addr);
	}

	std::string getServerName( void )
	{
		sockaddr_in	addr;
		socklen_t len = sizeof(addr);
		if (getsockname(conf.server_fd, (sockaddr *)&addr, &len) == -1)
			return "";//error
		return inet_ntoa(addr.sin_addr);
	}
	
	void addAllArgs( void )
	{
		args.push_back(conf.cgi_path);
		args.push_back(conf.translated_path);
	}

	char	*getInterpreterPath(const char *path, const char *cmd)
	{
		char	**paths;
		char	*file;
		int		i;

		if (!cmd)
			return (NULL);
		if (ft_check_args(path, cmd) == ERROR)
			return (NULL);
		if (ft_strrchr(cmd, '/'))
			return (ft_strdup(cmd));
		paths = ft_split(path, ':');
		if (!paths)
			return (ft_perror("malloc"), NULL);
		i = 0;
		while (paths[i])
		{
			file = ft_strjoin3(paths[i++], "/", cmd);
			if (!file)
				return (ft_clear(paths), ft_perror("malloc"), file);
			if (access(file, F_OK) == 0)
				return (ft_clear(paths), file);
			free(file);
		}
		ft_error(cmd, "command not found");
		return (g_data.exit_status = 127, ft_clear(paths), NULL);
	}

	// std::string getInterepreter( void )
	// {
	// 	std::string interpreter = conf.translated_path.find(".py") != std::string::npos ? "python" : "php";
	// 	size_t pos = interpreter.find_last_of('/');
	// 	if (pos != std::string::npos)
	// 		interpreter = interpreter.substr(pos + 1);
	// 	return interpreter;
	// }

	void execute( void )
	{
		
	}
};
