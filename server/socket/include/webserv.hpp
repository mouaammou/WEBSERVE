/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 23:53:05 by mouaammo          #+#    #+#             */
/*   Updated: 2024/02/18 20:30:38 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <stdio.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <dirent.h>

#include <csignal>
#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <ctime>

#include "../../config/include/config.hpp"

#define COLOR_RED     "\033[0;31m"
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_CYAN    "\033[0;36m"

#define PERMISSION_CHECK 0644
#define MAX_REQUEST_SIZE 50000

class Config;
class Location;
class Server;
class Request;
class PollServers;//may cause a problem if not included

typedef struct config
{
	std::string				uploaded_file_path;
    std::string				upload_location;
	std::vector<Location>	server_locations;
	std::string				server_name;
	std::string				request_url;
	std::string 			port;
	Directives 				*Server;
	Location 				location;
	std::string 			translated_path;//physical ==> root + requested_path
	std::string 			path_info;
	std::string 			path_info_translated;// physical ==> root + path_info
	std::string 			response_code;
	std::string 			requested_path;//location
	std::string				autoindex;//on or off
	Request					*request;

	bool					cgi;
	int						server_fd;
	int						body_size;
	int 					socket_fd;
}		t_config;

typedef struct aaa
{
    long startime;
}t_T_time;

#include <time.h>
#include <sys/time.h>

long getCurrentTimeInSeconds() ;


void				stringTrim(std::string &str);