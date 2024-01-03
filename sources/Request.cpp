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
#include "../Response/include/Response.hpp"
#include <cctype>

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
	this->query_string = "";
	this->_status_code = "200 OK";
	this->_body_size  = this->server_config.body_size > 0 ? this->server_config.body_size : -1;
}

Request::~Request()
{
	delete [] this->buffer;
}

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

std::map<std::string, std::string>& Request::getRequestHeaders()
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

bool			Request::hasBody() const
{
	return (this->_has_body);
}

int				Request::getFd() const
{
	return (this->fd);
}

void					 Request::setFd(int fd)
{
	this->fd = fd;
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

std::string 			 Request::getQueryString() const
{
	return (this->query_string);
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
	this->_status_code = "200 OK";
	this->query_string = "";
}

bool			      Request::handleBadRequest()
{
	if (this->isLocationHasRedirection())
		return (false);
	if (this->_status_code.find("400") != std::string::npos			//bad request
		|| this->_status_code.find("413") != std::string::npos		//request entity too large
		|| this->_status_code.find("414") != std::string::npos		//request uri too long
		|| this->_status_code.find("501") != std::string::npos		//not implemented
		|| this->_status_code.find("405") != std::string::npos		//method not allowed
		|| this->_status_code.find("403") != std::string::npos		//forbidden
		|| this->_status_code.find("505") != std::string::npos)		//http version not supported
		return (false);
	return (true);
}

bool 				   Request::isLocationHasRedirection()
{
	std::vector <Location> locations = this->server_config.server_locations;
	for (size_t i = 0; i < locations.size(); i++)
	{
		if (this->path == locations[i].getName())
		{
			if (locations[i].getReturnInt())
			{
				this->path = locations[i].getReturnString();
				this->_status_code = "301 Moved Permanently";
				return (true);
			}
		}
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
		std::cout << COLOR_GREEN << "Request Body: {{" << COLOR_RESET << this->request_body << "}}" << std::endl;
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
	std::stringstream lineStream(line);

	lineStream >> this->method >> this->path >> this->version;
	if (this->method.empty() || this->path.empty() || this->version.empty())
	{
		return (false);
	}
	this->checkMethod();
	this->checkVersion();
	this->checkPath();
	return (true);
}

bool	Request::parseRequestHeaders(const std::string& line)//hasheaders, requestheaders, contentlength
{
	std::size_t pos = line.find(':');
	if (pos != std::string::npos)
	{
		//HOST: localhost:8080/r\n
		std::string key = line.substr(0, pos + 1);
		if (key.find(" ") != std::string::npos || key.find(":") == std::string::npos)
			return (false);
		if (isdigit(key[0]))
			return (false);
		std::string value = line.substr(pos + 1);
		if (value.empty())
			return (false);
		if (key.compare("Content-Length:") == 0)//if the key is Content-Length
		{
			std::stringstream ss(value);
			ss >> this->content_length;
		}
		if (key.compare("Transfer-Encoding:") == 0)//if the key is Transfer-Encoding
		{
			if (value == " chunked\r\n")
			{
				this->transfer_encoding = value;
			}
			else
			{
				this->transfer_encoding = "error";
				this->_status_code = "501 Not Implemented";
				return (true);
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
	if (this->path.find("..") != std::string::npos)
		return (_status_code = "403 Forbidden", true);
	if (this->path.find("#") != std::string::npos)
		this->path = this->path.substr(0, this->path.find("#"));
	if (this->path.find("?") != std::string::npos)
	{
		this->query_string = this->path.substr(this->path.find("?") + 1);
		this->path = this->path.substr(0, this->path.find("?"));
	}
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

		ss >> std::hex >> decimal;
		mychar = static_cast<char>(decimal);
		this->path.replace(this->path.find("%"), 3, 1, mychar);
	}
	return (true);
}

bool	Request::checkVersion()
{
	if (this->version.compare("HTTP/1.1") != 0)
		this->_status_code = "505 HTTP Version Not Supported";
	return (true);
}

bool		Request::hasHeaders() const
{
	return (this->_has_headers);
}

bool Request::handleRequestHeaders(std::string bufferString)
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
			return (this->_status_code = "400 Bad Request", true);
		if (!parseRequestFirstLine(line))
			return (this->_status_code = "400 Bad Request", true);
		line = "";
	}
	while (std::getline(requestStream, line))
	{
		line += "\n";
		if (line.compare("\r\n") == 0)//end of headers
		{
			this->_has_headers = true;
			return (true);
		}
		size_t pos = line.find("\r\n");
		if (pos == std::string::npos)
			return (this->_status_code = "400 Bad Request", true);
		if (!parseRequestHeaders(line))
			return (this->_status_code = "400 Bad Request", true);
		line = "";
	}
	if (this->request_string.find("\r\n\r\n") == std::string::npos)//key: value\r\n\r\n
		this->_has_headers = false;
	return (true);
}


bool	Request::storeRequestBody()//hasbody, requestbody
{
	if (this->request_body == "")
	{
		std::string tmp = this->buffer;
		this->request_body += tmp.substr(tmp.find("\r\n\r\n") + 4);//key: value\r\n\r\n
	}
	else
		this->request_body += this->buffer;
	if (this->request_body.length() >= (this->content_length))
	{
		this->_has_body = true;
		return (true);
	}
	return (false);
}

bool			Request::storeChunkedRequestBody()
{
	if (this->request_body == "")
	{
		std::string tmp = this->buffer;
		this->request_body += tmp.substr(tmp.find("\r\n\r\n") + 4);
	}
	else
		this->request_body += this->buffer;
	if (this->request_body.find("0\r\n\r\n") != std::string::npos)
	{
		//substr first line and last line
		this->request_body = this->request_body.substr(this->request_body.find("\r\n") + 2);
		this->request_body = this->request_body.substr(0, this->request_body.find("0\r\n\r\n"));
		this->_has_body = true;
		return (true);
	}
	return (false);
}

bool	Request::receiveRequest()//must read the request
{
	int readStatus;
	memset(this->buffer, 0, MAX_REQUEST_SIZE + 1);
	readStatus = read(this->fd, this->buffer, MAX_REQUEST_SIZE);
	if (readStatus <= 0)
		return (perror("read"), this->read_bytes = 0, false);
	this->buffer[readStatus] = '\0';
	this->read_bytes += readStatus;
	if (this->request_string.find("\r\n\r\n") == std::string::npos)
	{	
		this->request_string += this->buffer;
		this->handleRequestHeaders(this->request_string);
		if (this->handleBadRequest() == false)
			return (true);
	}

	if (this->content_length > 0 || this->transfer_encoding.find("chunked") != std::string::npos)
	{
		if (this->_body_size != -1 && (int)this->request_body.length() > this->_body_size)
		{
			this->_status_code = "413 Request Entity Too Large";
			return (true);
		}
		
		if (this->hasHeaders() && this->transfer_encoding.find("chunked") != std::string::npos)
			return (this->storeChunkedRequestBody());
		if (this->hasHeaders() && this->content_length > 0)
			return (this->storeRequestBody());
	}

	if (this->hasHeaders() && this->content_length == 0 && this->transfer_encoding == "")
		return (true);
	return (false);
}


bool   Request::sendResponse()
{
	// bool ret = Response::onPollout(this->fd);
	// std::cout << "Request::sendResponse: " << (ret == true ? "tue" : "false") << std::endl;
	return (Response::onPollout(this->fd));
}
