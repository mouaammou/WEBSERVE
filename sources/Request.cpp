/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:16:59 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/11 18:12:36 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"
#include <sys/fcntl.h>

Request::Request(int fd, t_config config_file)
{
	this->fd = fd;
	this->server_config = config_file;
	this->requestString = "";
	this->method = "";
	this->path = "";
	this->version = "";
	this->requestBody = "";
	this->contentLength = 0;
	this->_hasHeaders = false;
	this->_hasBody = false;
	this->transferEncoding = "";
}

Request::~Request(){}//Default destructor

std::string Request::getMethod() const
{
	return (this->method);
}

std::string Request::getPath() const
{
	return (this->path);
}

std::string Request::getVersion() const
{
	return (this->version);
}

std::map<std::string, std::string> Request::getRequestHeaders() const
{
	return (this->requestHeaders);
}

std::string Request::getRequestBody() const
{
	return (this->requestBody);
}

size_t Request::getContentLength() const
{
	return (this->contentLength);
}

bool			Request::hasBody() const
{
	return (this->_hasBody);
}

int				Request::getFd() const
{
	return (this->fd);
}

void   Request::resetRequestState()
{
	this->requestString = "";
	this->method = "";
	this->path = "";
	this->version = "";
	this->requestBody = "";
	this->contentLength = 0;
	this->_hasHeaders = false;
	this->_hasBody = false;
}

//display request headers
void Request::displayRequest()
{
	if (!this->requestString.empty())
	{
		std::cout << COLOR_GREEN << "Method: " 			<< COLOR_RESET << this->method << std::endl;
		std::cout << COLOR_GREEN << "Path: " 			<< COLOR_RESET << this->path 	<< std::endl;
		std::cout << COLOR_GREEN << "Version: " 		<< COLOR_RESET << this->version << std::endl;
		std::cout << COLOR_GREEN << "Request Headers: " << COLOR_RESET << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = this->requestHeaders.begin(); it != this->requestHeaders.end(); ++it)
			std::cout << it->first << "=>" << it->second;
		std::cout << COLOR_GREEN << "Request Body: " << COLOR_RESET << this->requestBody << std::endl;
	}
}

bool	Request::allowedURIchars(std::string& str)
{
	std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]@!$&'()*+,;=";
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == ' ' || str[i] == '\t' || str[i] == '\r' || str[i] == '\n')
			return (false);
		if (allowedChars.find(str[i]) == std::string::npos)
			return (false);
	}
	return (true);
}

void	Request::parseRequestFirstLine(const std::string& line)
{
	std::istringstream lineStream(line);

	lineStream >> this->method >> this->path >> this->version;
	if (this->method.empty() || this->path.empty() || this->version.empty())
	{
		throw std::runtime_error("Invalid request line");
	}
	this->checkMethod();
	this->checkPath();
	this->checkVersion();
}

void	Request::parseRequestHeaders(const std::string& line)//hasheaders, requestheaders, contentlength
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
			ss >> this->contentLength;
		}
		if (key.compare("Transfer-Encoding:") == 0)//if the key is Transfer-Encoding
		{
			this->transferEncoding = value;
		}
		this->requestHeaders[key] = value;
	}
	if (this->requestHeaders.size() > 0)
		this->_hasHeaders = true;
}

void	Request::checkMethod()
{
	if (this->method.compare("GET") != 0 && this->method.compare("POST") != 0 && this->method.compare("DELETE") != 0)
		throw std::runtime_error("Invalid request method");
}

void Request::checkPath()
{
	if (this->allowedURIchars(this->path) == false)
		throw std::runtime_error("Invalid request path");
	while (this->path.find("%") != std::string::npos)
	{
		int decimal;
		char mychar;
		std::string tmp =  this->path.substr(this->path.find("%") + 1, 2);
		std::stringstream ss(tmp);

		ss >> std::hex >> decimal;//convert hex to decimal
		mychar = static_cast<char>(decimal);
		this->path.replace(this->path.find("%"), 3, 1, mychar);
	}
}

void	Request::requestFormatError()
{
	if (this->transferEncoding == "" && this->contentLength == 0 && this->method == "POST")
		throw std::runtime_error("Invalid request format");
	if (this->path.length() > 2048)
		throw std::runtime_error("Request-URI Too Long");
}

void	Request::checkVersion()
{
	if (this->version.compare("HTTP/1.1") != 0)
		throw std::runtime_error("Invalid request version");
}

bool		Request::hasHeaders() const
{
	return (this->_hasHeaders);
}

void Request::parseRequest(std::string bufferString)
{
	std::stringstream requestStream(bufferString);

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
			this->_hasHeaders = true;
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


void	Request::storeRequestBody(std::stringstream& requestStream)//hasbody, requestbody
{
	// Parse the body
	std::string line;
	if (this->contentLength > 0)
	{
		this->_hasBody = true;
		while (getline(requestStream, line))
		{
			line += "\n";
			this->requestBody += line;
			line.clear();
		}
		std::cout << COLOR_CYAN "Receive the body \n" COLOR_RESET << std::endl;
	}
	else
		std::cout << COLOR_CYAN "No request body" COLOR_RESET << std::endl;
}

bool	Request::receiveRequest()//must read the request
{
	char *buffer = new char[MAX_REQUEST_SIZE];
	memset(buffer, 0, MAX_REQUEST_SIZE);//clear the buffer
	int readBytes = read(this->fd, buffer, MAX_REQUEST_SIZE - 1);
	if (readBytes < 0)
	{
		perror("read");
		return (false);
	}
	if (readBytes == 0)
	{
		std::cout << COLOR_RED "Request disconnected" COLOR_RESET << std::endl;
		return (true);
	}
	this->requestString += buffer;
	this->parseRequest(this->requestString);
	delete [] buffer;

	if (this->hasHeaders() && this->contentLength == 0)
	{
		std::cout << COLOR_CYAN "End of request" COLOR_RESET << std::endl;
		return (true);
	}
	else if (this->hasHeaders() && this->hasBody())
	{
		if (this->requestBody.length() == (this->contentLength + 1))
		{
			std::cout << COLOR_CYAN "End of request" COLOR_RESET << std::endl;
			return (true);
		}
		else
		{
			std::cout << COLOR_CYAN "Waiting for the body" COLOR_RESET << std::endl;
			return (false);
		}
	}
	std::cout << COLOR_CYAN "Waiting for the headers" COLOR_RESET << std::endl;
	return (false);
}


// Response

bool   Request::sendResponse()
{
	int pageFd = open("/Users/mouaammo/Desktop/WEBSERVE/html/index.html", O_RDWR);
	if (pageFd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	int fileSize = get_file_size(pageFd);
	off_t len = fileSize;

	std::string responseHeader = "HTTP/1.1 200 OK\r\n";
		responseHeader += "Content-Type: text/html\r\n";
		responseHeader += "Content-Length: " + std::to_string(fileSize) + "\r\n";
		responseHeader += "\r\n";
	if (send(this->fd, responseHeader.c_str(), responseHeader.length(), 0) == -1)
	{
		perror("send");
		return false;
	}
	if (sendfile(pageFd, this->fd, 0, &len, NULL, 0) == -1)
	{
		perror("sendfile");
		return false;
	}
	close(pageFd);
	return (true);
}

int Request::get_file_size(int fd)
{
    int file_size;
    int current_position = lseek(fd, 0, SEEK_CUR); // Get the current position
    file_size = lseek(fd, 0, SEEK_END); // Seek to the end of the file
    lseek(fd, current_position, SEEK_SET); // Return to the original position
    return file_size;
}
