/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/07 00:12:15 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/07 21:41:02 by mouaammo         ###   ########.fr       */
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

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[0;36m"

#define PERMISSION_CHECK 0644
#define MAX_REQUEST_SIZE 50000

class Client
{
	private:

		int fd;
		
		std::string requestString;
		std::string method;
		std::string path;
		std::string version;
		std::string requestBody;
		std::map<std::string, std::string> requestHeaders;
		size_t contentLength;
		bool _hasHeaders;
		bool _hasBody;

		size_t sendBytes;
		std::string responseHeader;
		std::string responseBody;
		size_t	responseBodySize;
		bool isSendBody;
		bool isSendHeader;
		
	public:
		// Constructor to initialize the object with the raw HTTP request
		Client(int fd);
		~Client();
		// Getters to retrieve information from the parsed request
		std::string 	getMethod() const;
		std::string 	getPath() const;
		std::string 	getVersion() const;
		std::string 	getRequestBody() const;
		size_t			getContentLength() const;
		bool			hasSendBody() const;
		bool			hasSendHeader() const;
		bool			hasHeaders() const;
		bool			hasBody() const;
		int				getFd() const;
		std::map<std::string, std::string>	getRequestHeaders() const;


		//display request headers
		void	displayRequest();
		// Function to parse the raw HTTP request
		void	parseRequest(std::string bufferString);
		void	parseRequestFirstLine(const std::string& line);
		void	parseRequestHeaders(const std::string& line);
		void	storeRequestBody(std::stringstream& requestStream);


		void	checkMethod();
		void	checkPath();
		void	checkVersion();
		void	checkRequestHeaders();
		void	parseURIencoded();

		bool   receiveRequest();
		bool   sendResponse();

		void   resetResponseState();
};
