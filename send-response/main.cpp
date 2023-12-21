/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/20 22:02:22 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 01:04:09 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>
#include "../SendResponse.cpp"

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


class Basic
{
	public:

	// static t_args			args;

	static void CGI( t_args *args )
	{
		// CGI::build( args );
	}

	static void AutoIndex( t_args *args )
	{
		// AutoIndex(args->translated_path, args->sfd);
	}

	static void File( t_args *args )
	{
		Basic basic(*args);
		basic.file();
	}

	static void Redirect( t_args *args )
	{
		Basic basic(*args);
		basic.redirect();
	}

	static void Error( t_args *args )
	{
		Basic basic(*args);
		basic.error();
	}

	private:

	std::ostringstream	oss;
	int					ffd;
	t_args				args;

	void statusLine( std::string code )
	{
		oss << "HTTP/1.1 " << "200" << " " << "OK TEST" << "\r\n";
	}

	int64_t get_file_size( void )
	{
		if (ffd == -1)
			return -1;
		struct stat stat_buf;
		if (fstat(ffd, &stat_buf) != 0)
		{
			close(ffd);
			return -1;
		}
		return stat_buf.st_size;
	}

	public:

	Basic( t_args &args )
	{
		this->args = args;
	}

	void file( void )
	{
		ffd = open(args.translated_path.c_str(), O_RDONLY);
		int64_t file_size = get_file_size();
		if (file_size == -1)
		{
			args.code = "500";
			error();
			return ;
		}
		statusLine(args.code);
		oss << "Content-Length: " << file_size << "\r\n";
		oss << "Content-Type: " << args.location.getType() << "\r\n";
		oss << "Cache-Control: no-store\r\n";
		oss << "\r\n";
		SendResponse(oss.str(), ffd, args.sfd);
	}

	void redirect( void )
	{
		statusLine(args.code);
		oss << "Location: " << args.translated_path << "\r\n";
		oss << "\r\n";
		SendResponse(oss.str(), -1, args.sfd);
	}

	void error( void )//5xx 4xx
	{
		ffd = open(args.translated_path.c_str(), O_RDONLY);
		int64_t file_size = get_file_size();
		if (file_size == -1)
		{
			statusLine("500");
			oss << "\r\n";
			SendResponse(oss.str(), -1, args.sfd);
			return ;
		}
		statusLine(args.code);
		oss << "Content-Length: " << file_size << "\r\n";
		oss << "Content-Type: " << args.location.getType() << "\r\n";
		oss << "Cache-Control: no-store\r\n";
		oss << "\r\n";
		SendResponse(oss.str(), ffd, args.sfd);
	}
};
