/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/06 16:15:49 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/06 16:19:03 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Config.hpp"

config::config( void )
{
}

config::config( const config &args )
{
	server_locations = args.server_locations;
	server_name = args.server_name;
	port = args.port;
	Server = args.Server;
	location = args.location;
	translated_path = args.translated_path;
	response_code = args.response_code;
	requested_path = args.requested_path;
	autoindex = args.autoindex;
	request = args.request;
	cgi = args.cgi;
	server_fd = args.server_fd;
	body_size = args.body_size;
}