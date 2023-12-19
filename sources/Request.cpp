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
	this->request_string = "";
	this->method = "";
	this->path = "";
	this->version = "";
	this->request_body = "";
	this->content_length = 0;
	this->_has_headers = false;
	this->_has_body = false;
	this->transfer_encoding = "";
	this->read_bytes = 0;
	this->content_type = "";
	this->buffer = new char[MAX_REQUEST_SIZE + 1];
	this->request_received = false;
	this->_requested_method = NULL;
	this->_status_code = "";
	this->_body_size  = this->server_config.body_size;
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
	return (this->request_headers);
}

std::string Request::getRequestBody() const
{
	return (this->request_body);
}

int				Request::getReadBytes() const
{
	return (this->read_bytes);
}

size_t Request::getContentLength() const
{
	return (this->content_length);
}

std::string 		Request::getTransferEncoding() const
{
	return (this->transfer_encoding);
}

Method* 		Request::getRequestedMethod() const
{
	return (this->_requested_method);
}

bool			Request::hasBody() const
{
	return (this->_has_body);
}

int				Request::getFd() const
{
	return (this->fd);
}

std::string 		  Request::getStatusCode() const
{
	return (this->_status_code);
}

bool		 Request::hasRequest() const
{
	return (this->request_received);
}

void		 Request::setRequestReceived(bool request_received)
{
	this->request_received = request_received;
}

void   Request::resetRequestState()
{
	this->request_string = "";
	this->request_body = "";
	this->method = "";
	this->path = "";
	this->version = "";
	this->content_length = 0;
	this->_has_headers = false;
	this->_has_body = false;
	this->read_bytes = 0;
	this->content_type = "";
	this->transfer_encoding = "";
	this->buffer[0] = '\0';
	this->request_received = false;
}

bool			      Request::handleBadRequest()
{
	if (this->checkRequestLocation() == false)
	{
		this->_status_code = "404 Not Found";
		return (false);
	}
	if (this->_status_code.find("400") != std::string::npos
		|| this->_status_code.find("413") != std::string::npos
		|| this->_status_code.find("414") != std::string::npos
		|| this->_status_code.find("501") != std::string::npos
		|| this->_status_code.find("405") != std::string::npos)
		return (false);
	return (true);
}

bool 				   Request::checkRequestLocation()
{
	std::vector <Location> locations = this->server_config.server_locations;
	for (size_t i = 0; i < locations.size(); i++)
	{
		if (this->path == locations[i].getName())
			return (true);
	}
	return (false);
}

//display request headers
void Request::displayRequest()
{
	if (!this->request_string.empty())
	{
		std::cout << COLOR_GREEN << "Method: " 			<< COLOR_RESET << this->method << std::endl;
		std::cout << COLOR_GREEN << "Path: " 			<< COLOR_RESET << this->path 	<< std::endl;
		std::cout << COLOR_GREEN << "Version: " 		<< COLOR_RESET << this->version << std::endl;
		std::cout << COLOR_GREEN << "Request Headers: " << COLOR_RESET << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = this->request_headers.begin(); it != this->request_headers.end(); ++it)
			std::cout << it->first << "=>" << it->second;
		std::cout << COLOR_GREEN << "Request Body: " << COLOR_RESET << this->request_body << std::endl;
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

bool	Request::parseRequestFirstLine(const std::string& line)
{
	std::istringstream lineStream(line);

	lineStream >> this->method >> this->path >> this->version;
	if (this->method.empty() || this->path.empty() || this->version.empty())
	{
		return (false);
	}
	if (!this->checkMethod() || !this->checkVersion() || !this->checkPath())
		return (false);
	return (true);
}

bool	Request::parseRequestHeaders(const std::string& line)//hasheaders, requestheaders, contentlength
{
	std::size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		std::string key = line.substr(0, pos + 1);
		if (key.find(" ") != std::string::npos || key.find(":") == std::string::npos)
			return (false);
		std::string value = line.substr(pos + 1);

		if (key.compare("Content-Length:") == 0)//if the key is Content-Length
		{
			std::stringstream ss(value);
			ss >> this->content_length;
		}
		if (key.compare("Transfer-Encoding:") == 0)//if the key is Transfer-Encoding
		{
			if (value == "chunked")
				this->transfer_encoding = value;
			else
			{
				this->transfer_encoding = "error";
				this->_status_code = "501 Not Implemented";
			}
		}
		if (key.compare("Content-Type:") == 0)//if the key is Content-Type
		{
			this->content_type = value;
		}
		this->request_headers[key] = value;
	}
	return (true);
}

bool	Request::checkMethod()
{
	if (this->method.compare("GET") != 0 && this->method.compare("POST") != 0 && this->method.compare("DELETE") != 0)
		this->_status_code = "405 Method Not Allowed";
	return (true);
}

bool Request::checkPath()
{
	if (this->allowedURIchars(this->path) == false)
	{
		this->_status_code = "400 Bad Request";
		return (true);
	}
	if (this->path.length() > 2048)
	{
		this->_status_code = "414 Request-URI Too Long";
		return (true);
	}
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
	return (true);
}

bool	Request::checkVersion()
{
	if (this->version.compare("HTTP/1.1") != 0)
		return (false);
	return (true);
}

bool		Request::hasHeaders() const
{
	return (this->_has_headers);
}

bool Request::handleRequestHeader(std::string bufferString)
{
	if (this->_has_headers)
		return (true);
	std::stringstream requestStream;
	requestStream << bufferString;

	std::string line;
	if (std::getline(requestStream, line))
	{
		line += "\n";
		size_t pos = line.find("\r\n");
		if (pos == std::string::npos)
			return (false);
		if (!parseRequestFirstLine(line))
			return (false);
		line = "";
	}
	// Parse the headers
	while (std::getline(requestStream, line))
	{
		line += "\n";
		if (line.compare("\r\n") == 0)
		{
			std::cout << COLOR_YELLOW "THE END OF HEADERS" COLOR_RESET << std::endl;
			this->_has_headers = true;
			return (true);
		}
		size_t pos = line.find("\r\n");
		if (pos == std::string::npos)
			return (false);
		if (!parseRequestHeaders(line))
			return (false);
		line = "";
	}
	if (this->request_string.find("\r\n\r\n") == std::string::npos)
		return (false);
	return (true);
}


bool	Request::storeRequestBody()//hasbody, requestbody
{
	if (this->hasHeaders() && this->content_length > 0)
	{
		if (this->request_body == "")
		{
			std::string tmp = this->buffer;
			this->request_body += this->request_string.substr(tmp.find("\r\n\r\n") + 4);
		}
		else
			this->request_body += this->buffer;
		if (this->request_body.length() >= (this->content_length))
		{
			printf("content length: %zu\n", this->content_length);
			printf("request body length: %zu\n", this->request_body.length());
			std::cout << COLOR_YELLOW "THE END OF BODY" COLOR_RESET << std::endl;
			this->_has_body = true;
			return (true);
		}
	}
	return (false);
}

bool			Request::storeChunkedRequestBody()
{
	if (this->hasHeaders() && this->transfer_encoding == "chunked")
	{
		if (this->request_body == "")
			this->request_body += this->request_string.substr(this->request_string.find("\r\n\r\n") + 4) + this->buffer;
		else
			this->request_body += this->buffer;
		if (this->request_body.find("0\r\n\r\n") != std::string::npos)
		{
			this->_has_body = true;
			return (true);
		}
	}
	return (false);
}

bool	Request::receiveRequest()//must read the request
{
	int readStatus;
	readStatus = read(this->fd, this->buffer, MAX_REQUEST_SIZE);
	if (readStatus <= 0)
		return (perror("read"), this->read_bytes = 0, false);
	this->buffer[readStatus] = '\0';
	this->read_bytes += readStatus;
	if (this->request_string.find("\r\n\r\n") == std::string::npos)
	{
		this->request_string += this->buffer;
		if (!this->handleRequestHeader(this->request_string))
			return (false);
		if (this->handleBadRequest() == false)
			return (true);
	}
	if (this->content_length > 0 || this->transfer_encoding == "chunked")
	{
		if ((int)this->request_body.length() > this->_body_size)
		{
			this->_status_code = "413 Request Entity Too Large";
			return (true);
		}
		return (this->storeRequestBody() || this->storeChunkedRequestBody());
	}

	if (this->hasHeaders() && this->content_length == 0 && this->transfer_encoding == "")
		return (true);
	return (false);
}


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
