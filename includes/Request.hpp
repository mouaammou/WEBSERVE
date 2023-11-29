/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:11:44 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/28 21:39:18 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <sstream>
#include <sys/stat.h>

#define COLOR_RED     "\033[0;31m"
#define COLOR_GREEN   "\033[0;32m"
#define COLOR_BLUE    "\033[0;34m"
#define COLOR_YELLOW  "\033[0;33m"
#define COLOR_MAGENTA "\033[0;35m"
#define COLOR_RESET   "\033[0m"
#define COLOR_CYAN    "\033[0;36m"
#define PERMISSION_CHECK 0644

class Request
{
	private:
		std::string RequestString;
		std::string Method;
		std::string Path;
		std::string Version;
		std::map<std::string, std::string> RequestHeaders;
		std::string RequestBody;
		
	public:
		// Constructor to initialize the object with the raw HTTP request
		Request(std::string requestString);
		~Request();
		// Getters to retrieve information from the parsed request
		std::string getMethod() const;
		std::string getPath() const;
		std::string getVersion() const;
		std::map<std::string, std::string> getRequestHeaders() const;
		std::string getRequestBody() const;


		//display request headers
		void	displayRequestHeaders() const;
		// Function to parse the raw HTTP request
		void	parseRequest();
		void	parseRequestFirstLine(const std::string& line);
		void	parseRequestHeaders(const std::string& line);
		void	parseRequestBody(const std::string& line);
		
		void	checkMethod();
		void	checkPath();
		void	checkVersion();
		void	checkRequestHeaders();
		void	parseURIencoded();
};
