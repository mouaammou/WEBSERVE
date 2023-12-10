/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/23 13:16:59 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/10 18:35:02 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Client.hpp"

Client::Client(int fd)
{
	this->fd = fd;
	this->requestString = "";
	this->method = "";
	this->path = "";
	this->version = "";
	this->requestBody = "";
	this->contentLength = 0;
	this->_hasHeaders = false;
	this->_hasBody = false;
	this->sendBytes = 0;
	this->isSendBody = false;
	this->isSendHeader = false;
	this->responseBodySize = 0;
}

Client::~Client(){}//Default destructor

std::string Client::getMethod() const
{
	return (this->method);
}

std::string Client::getPath() const
{
	return (this->path);
}

std::string Client::getVersion() const
{
	return (this->version);
}

std::map<std::string, std::string> Client::getRequestHeaders() const
{
	return (this->requestHeaders);
}

std::string Client::getRequestBody() const
{
	return (this->requestBody);
}

size_t Client::getContentLength() const
{
	return (this->contentLength);
}

bool			Client::hasBody() const
{
	return (this->_hasBody);
}

bool			Client::hasSendBody() const
{
	return (this->isSendBody);
}

bool			Client::hasSendHeader() const
{
	return (this->isSendHeader);
}

int				Client::getFd() const
{
	return (this->fd);
}

void   Client::resetResponseState()
{
	this->sendBytes = 0;
	this->isSendBody = false;
	this->isSendHeader = false;
	this->responseBodySize = 0;
}

void   Client::resetRequestState()
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
void Client::displayRequest()
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

void	Client::parseRequestFirstLine(const std::string& line)
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

void	Client::parseRequestHeaders(const std::string& line)//hasheaders, requestheaders, contentlength
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
		this->requestHeaders[key] = value;
	}
	if (this->requestHeaders.size() > 0)
		this->_hasHeaders = true;
}

void	Client::checkMethod()
{
	if (this->method.compare("GET") != 0 && this->method.compare("POST") != 0 && this->method.compare("DELETE") != 0)
		throw std::runtime_error("Invalid request method");
}

void Client::checkPath()
{
	struct stat info;
	stat(this->path.c_str(), &info);
	if (S_ISDIR(info.st_mode))
	{
		if (this->path[this->path.length() - 1] != '/')
			this->path += "/";
		this->path += "index.html";
	}
	this->parseURIencoded();
}

void	Client::parseURIencoded()
{
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

void	Client::checkVersion()
{
	if (this->version.compare("HTTP/1.1") != 0)
		throw std::runtime_error("Invalid request version");
}

bool		Client::hasHeaders() const
{
	return (this->_hasHeaders);
}

void Client::parseRequest(std::string bufferString)
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


void	Client::storeRequestBody(std::stringstream& requestStream)//hasbody, requestbody
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

bool	Client::receiveRequest()
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
		std::cout << COLOR_CYAN "Client disconnected" COLOR_RESET << std::endl;
		return (false);
	}
	this->requestString += buffer;
	this->parseRequest(this->requestString);
	delete [] buffer;

	printf("requestBody.length() = %zu\n", this->requestBody.length());
	printf("contentLength = %zu\n", this->contentLength);
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

bool	Client::sendHeader()
{
	if (!this->isSendHeader)
	{
		this->responseHeader = "HTTP/1.1 200 OK\r\n";
		this->responseHeader += "Content-Type: text/html\r\n";
		this->responseHeader += "Content-Length: " + std::to_string(this->responseBodySize) + "\r\n";
		this->responseHeader += "\r\n";
		int sendStatus = send(this->fd, this->responseHeader.c_str(), this->responseHeader.length(), 0);
		if (sendStatus <= 0)
		{
			perror("send");
			return (false);
		}
		this->isSendHeader = true;
	}

	return (true);
}

bool	Client::sendPage(int pageFd, size_t pageSize)
{
	off_t len = 1024 * 2;

	if (this->isSendBody == false)
	{
		if (sendfile(pageFd, this->fd, this->sendBytes, &len, NULL, 0) == -1)
		{
			perror("sendfile");
			return false;
		}
		this->sendBytes += len;
		
		printf("sendBytes = %zu\n", this->sendBytes);
		printf("pageSize = %zu\n", pageSize);
		if (this->sendBytes == pageSize)
		{
			this->isSendBody = true;
			std::cout << COLOR_CYAN "End of response" COLOR_RESET << std::endl;
			return true;
		}
	}
	return (false);
}

int Client::get_file_size(int fd)
{
    int file_size;
    int current_position = lseek(fd, 0, SEEK_CUR); // Get the current position
    file_size = lseek(fd, 0, SEEK_END); // Seek to the end of the file
    lseek(fd, current_position, SEEK_SET); // Return to the original position
    return file_size;
}

bool   Client::sendResponse(int pageFd)
{
	this->responseBodySize = get_file_size(pageFd);
	if (!this->sendHeader())
		return (false);

	if (!this->sendPage(pageFd, this->responseBodySize))
		return (false);
	lseek(pageFd, 0, SEEK_SET);
	return (true);
}
