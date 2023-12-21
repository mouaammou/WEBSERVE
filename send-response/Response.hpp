/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 01:08:15 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 21:44:53 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
#include <map>
#include <sys/stat.h>
#include <sys/types.h>
#include "SendResponse.hpp"
#include "AutoIndex.hpp"
#include "CGI.hpp"

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

	static void CGI( t_args *args );
	static void Autoindex( t_args *args );
	static void File( t_args *args );
	static void Redirect( t_args *args );
	static void Error( t_args *args );

	private:

	std::ostringstream	oss;
	int					ffd;
	t_args				args;

	void statusLine( std::string code );
	int64_t get_file_size( void );

	public:

	Basic( t_args &args );

	void file( void );
	void redirect( void );
	void error( void );
};