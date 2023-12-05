/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:16:59 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/05 18:46:06 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"
#include <cstdio>
#include <sys/_types/_size_t.h>

Request::Request(std::string requestString)//Default constructor
{
	this->RequestString = requestString;
	this->Method = "";
	this->Path = "";
	this->Version = "";
	parseRequest();
}

Request::~Request(){}//Default destructor

std::string Request::getMethod() const
{
	return (this->Method);
}

std::string Request::getPath() const
{
	return (this->Path);
}

std::string Request::getVersion() const
{
	return (this->Version);
}

std::map<std::string, std::string> Request::getRequestHeaders() const
{
	return (this->RequestHeaders);
}

std::string Request::getRequestBody() const
{
	return (this->RequestBody);
}

//display request headers
void Request::displayRequestHeaders() const
{
	std::cout << COLOR_GREEN << "Method: " << COLOR_RESET << this->Method << std::endl;
	std::cout << COLOR_GREEN << "Path: " << COLOR_RESET << this->Path << std::endl;
	std::cout << COLOR_GREEN << "Version: " << COLOR_RESET << this->Version << std::endl;
	std::cout << COLOR_GREEN << "Request Headers: " << COLOR_RESET << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = this->RequestHeaders.begin(); it != this->RequestHeaders.end(); ++it)
		std::cout << it->first << "=>" << it->second << std::endl;
	std::cout << COLOR_GREEN << "Request Body: " << COLOR_RESET << this->RequestBody << std::endl;
}

void Request::parseRequestFirstLine(const std::string& line)
{
	std::istringstream lineStream(line);

	lineStream >> this->Method >> this->Path >> this->Version;
	if (this->Method.empty() || this->Path.empty() || this->Version.empty())
	{
		throw std::runtime_error("Invalid request line");
	}
	this->checkMethod();
	this->checkPath();
	this->checkVersion();
}

void	Request::parseRequestHeaders(const std::string& line)
{
	std::size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		std::string key = line.substr(0, pos + 1);
		if (key.find(" ") != std::string::npos || key.find(":") == std::string::npos)
			throw std::runtime_error("Invalid header Key");
		std::string value = line.substr(pos + 1);

		if (key.compare("Content-Length:") == 0)//if the key is Content-Length
		{
			std::stringstream ss(value);
			ss >> this->ContentLength;
		}
		this->RequestHeaders[key] = value;
	}
}

void	Request::checkMethod()
{
	if (this->Method.compare("GET") != 0 && this->Method.compare("POST") != 0 && this->Method.compare("DELETE") != 0)
		throw std::runtime_error("Invalid request method");
}

void Request::checkPath()
{
	struct stat info;
	stat(this->Path.c_str(), &info);
	if (S_ISDIR(info.st_mode))
	{
		if (this->Path[this->Path.length() - 1] != '/')
			this->Path += "/";
		this->Path += "index.html";
	}
	this->parseURIencoded();
}

void	Request::parseURIencoded()
{
	if (this->Path.find("%") != std::string::npos)
	{
		int decimal;
		char mychar;
		std::string tmp =  this->Path.substr(this->Path.find("%") + 1, 2);
		std::stringstream ss(tmp);

		ss >> std::hex >> decimal;
		mychar = static_cast<char>(decimal);
		this->Path.replace(this->Path.find("%"), 3, 1, mychar);
		std::cout << COLOR_CYAN "Path after replacing: " COLOR_RESET << this->Path << std::endl;
	}
}

void	Request::checkVersion()
{
	if (this->Version.compare("HTTP/1.1") != 0)
		throw std::runtime_error("Invalid request version");
}

void Request::parseRequest()
{
	std::stringstream requestStream(this->RequestString);

	// Parse the request line
	std::string line;
	if (std::getline(requestStream, line))
	{
		size_t pos = line.find("\r\n");
		if (pos == std::string::npos)
			throw std::runtime_error("Invalid request line");
		parseRequestFirstLine(line);
	}
	// Parse the headers
	while (std::getline(requestStream, line))
	{
		if (line.empty() || line == "\r\n")
		{
			std::cout << COLOR_CYAN "End of request headers" COLOR_RESET << std::endl;
			break;
		}
		size_t pos = line.find("\r\n");
		if (pos == std::string::npos)
			throw std::runtime_error("Invalid request Header");
		parseRequestHeaders(line);
	}
	//parse the body
	storeRequestBody(requestStream);
}


void	Request::storeRequestBody(std::stringstream& requestStream)
{
	// Parse the body
	std::string line;
	if (this->ContentLength > 0)
	{
		while (getline(requestStream, line))
		{
			if (line.empty() || line == "\r\n")
			{
				std::cout << COLOR_CYAN "End of request body" COLOR_RESET << std::endl;
				break;
			}
			size_t pos = line.find("\r\n");
			if (pos == std::string::npos)
				throw std::runtime_error("Invalid request body");
			this->RequestBody += line;
		}
	}
}
