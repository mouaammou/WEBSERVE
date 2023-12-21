/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:22:50 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 01:36:55 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

void Basic::CGI( t_args *args )
{
	CGI::build( args );
}

void Basic::Autoindex( t_args *args )
{
	// need uri for the "Index of /uri" title
	AutoIndex autoindex(args->sfd, args->translated_path, args->request->uri); // make sure to pass the right uri
	if (autoindex.fail())
	{
		args->code = "500";
		Error(args);
		return ;
	}
	SendResponse(autoindex.getOutput(), -1, args->sfd);
}

void Basic::File( t_args *args )
{
	Basic basic(*args);
	basic.file();
}

void Redirect( t_args *args )
{
	Basic basic(*args);
	basic.redirect();
}

void Basic::Error( t_args *args )
{
	Basic basic(*args);
	basic.error();
}

void Basic::statusLine( std::string code )
{
	oss << "HTTP/1.1 " << "200" << " " << "OK TEST" << "\r\n";
}

int64_t Basic::get_file_size( void )
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

Basic::Basic( t_args &args )
{
	this->args = args;
}

void Basic::file( void )
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
	oss << "Content-Type: " << args.location->getType() << "\r\n";
	oss << "Cache-Control: no-store\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), ffd, args.sfd);
}

void Basic::redirect( void )
{
	statusLine(args.code);
	oss << "Location: " << args.translated_path << "\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), -1, args.sfd);
}

void Basic::error( void )//5xx 4xx
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
	oss << "Content-Type: " << args.location->getType() << "\r\n";
	oss << "Cache-Control: no-store\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), ffd, args.sfd);
}
