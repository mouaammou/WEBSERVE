/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:16:59 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/06 17:03:44 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

Request::Request(std::string requestString)
{
	this->RequestString = requestString;
	this->Method = "";
	this->Path = "";
	this->Version = "";
	this->RequestBody = "";
	this->ContentLength = 0;
	this->isRecvHeaders = false;
	this->isRecvBody = false;
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

size_t Request::getContentLength() const
{
	return (this->ContentLength);
}

bool			Request::getIsRecvBody() const
{
	return (this->isRecvBody);
}

//display request headers
void Request::displayRequestHeaders()
{
	if (!this->RequestString.empty())
	{
		std::cout << COLOR_GREEN << "Method: " << COLOR_RESET << this->Method << std::endl;
		std::cout << COLOR_GREEN << "Path: " << COLOR_RESET << this->Path << std::endl;
		std::cout << COLOR_GREEN << "Version: " << COLOR_RESET << this->Version << std::endl;
		std::cout << COLOR_GREEN << "Request Headers: " << COLOR_RESET << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = this->RequestHeaders.begin(); it != this->RequestHeaders.end(); ++it)
			std::cout << it->first << "=>" << it->second;
		std::cout << COLOR_GREEN << "Request Body: " << COLOR_RESET << this->RequestBody << std::endl;
	}
}

void	Request::parseRequestFirstLine(const std::string& line)
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

bool		Request::getIsRecvHeaders() const
{
	return (this->isRecvHeaders);
}

void Request::parseRequest()
{
	std::stringstream requestStream(this->RequestString);

	// Parse the request line
	std::string line;
	if (std::getline(requestStream, line))
	{
		line += "\n";
		size_t pos = line.find("\r\n");
		if (pos == std::string::npos)
			throw std::runtime_error("Invalid request line");
		parseRequestFirstLine(line);
		line.clear();
	}
	// Parse the headers
	while (std::getline(requestStream, line))
	{
		line += "\n";
		if (line.compare("\r\n") == 0)
		{
			std::cout << COLOR_CYAN "End of request headers" COLOR_RESET << std::endl;
			this->isRecvHeaders = true;
			break;
		}
		size_t pos = line.find("\r\n");
		if (pos == std::string::npos)
			throw std::runtime_error("Invalid request Header");
		parseRequestHeaders(line);
		line.clear();
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
		this->isRecvBody = true;
		while (getline(requestStream, line))
		{
			line += "\n";
			this->RequestBody += line;
			line.clear();
		}
		std::cout << COLOR_CYAN "Reive the body \n" COLOR_RESET << std::endl;
	}
	else
	{
		std::cout << COLOR_CYAN "No request body" COLOR_RESET << std::endl;
	}
}
