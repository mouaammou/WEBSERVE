/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 23:53:05 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/18 23:20:19 by mouaammo         ###   ########.fr       */
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

#include <csignal>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

#include "../config/config.hpp"

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[0;36m"

#define PERMISSION_CHECK 0644
#define MAX_REQUEST_SIZE 1024
class Config;
class Location;

typedef struct config
{
	std::vector<Location> server_locations;
	std::string server_name;
	std::string port;
	Location *location;
	std::string translated_path;
	std::string host_name;
	int	server_fd;
	int body_size;
}		t_config;
