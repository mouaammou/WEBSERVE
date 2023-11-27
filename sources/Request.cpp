/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:16:59 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/27 10:40:51 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"

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
		std::cout << it->first << " => " << it->second << std::endl;
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
		std::string value = line.substr(pos + 1);
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
	//replace / with ./ to access the current directory

	if (this->Path[0] == '/')
		this->Path = "." + this->Path;
	// std::cout << COLOR_RED "Path: " COLOR_RESET << this->Path << std::endl;
    // Check existence
    if (stat(this->Path.c_str(), &info) != 0)
	{
        // throw std::runtime_error("Error accessing the path");
		std::cout << COLOR_RED "Error accessing the path" COLOR_RESET << std::endl;
	}

    // Check if it's a directory
    if (S_ISDIR(info.st_mode)) {
        std::cout << COLOR_YELLOW "The path is a directory." COLOR_RESET << std::endl;
    } else if (S_ISREG(info.st_mode)) {
        std::cout << COLOR_YELLOW "The path is a regular file." COLOR_RESET << std::endl;
    } else {
		std::cout << COLOR_RED "The path is neither a directory nor a regular file." COLOR_RESET << std::endl;
        // throw std::runtime_error("The path is neither a directory nor a regular file.");
    }
	
    if ((info.st_mode & PERMISSION_CHECK) != 0) {
        std::cout << "The program has the necessary permissions." << std::endl;
    } else {
        throw std::runtime_error("Insufficient permissions to access the path.");
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
		parseRequestFirstLine(line);
	}

	while (std::getline(requestStream, line) && !line.empty())
	{
		if (line.compare("\r\n") == 0)
			break;
		parseRequestHeaders(line);
	}
}

