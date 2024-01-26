/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 03:50:35 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/26 12:10:45 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Request.hpp"
#include "../../Response/include/Response.hpp"

Request::Request(int fd, t_config config_file)
{
	this->fd = fd;
	read_bytes = -1;
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
	this->content_type = "";
	this->buffer = new char[MAX_REQUEST_SIZE + 1];
	this->request_received = false;
	this->query_string = "";
	this->_status_code = "200 OK";
	this->_body_size  = this->server_config.body_size;
	this->server_config.path_info = "";
	this->_connection = "";
	this->_chunked_body = "";
	this->_chunk_size = -1;
	this->_last_chunk_pos = 0;
}

void	Request::resetRequest()
{
	this->_chunked_body = "";
	this->request_string = "";
	this->_connection = "";
	this->method = "";
	this->path = "";
	this->version = "";
	this->request_body = "";
	this->transfer_encoding = "";
	this->content_length = 0;
	this->_has_headers = false;
	this->_has_body = false;
	this->content_type = "";
	this->request_received = false;
	this->query_string = "";
	this->_body_size  = this->server_config.body_size;
	this->_status_code = "200 OK";
	this->server_config.path_info = "";
	this->request_headers.clear();
	this->_chunk_size = -1;
	this->_last_chunk_pos = 0;
}

Request::~Request()
{
	delete [] this->buffer;
	this->server_config.request = NULL;
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

std::string 			Request::getContentType() const
{
	return (this->content_type);
}

std::string			 &Request::getRequestBody()
{
	return (this->request_body);
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
				std::stringstream ss;
				ss << locations[i].get_return_status();
				this->_status_code = ss.str();//"config file redirection";
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
		// std::cout << COLOR_GREEN << "Method: " 			<< COLOR_RESET << this->method << std::endl;
		// std::cout << COLOR_GREEN << "Path: " 			<< COLOR_RESET << this->path 	<< std::endl;
		// std::cout << COLOR_GREEN << "Version: " 		<< COLOR_RESET << this->version << std::endl;
		std::cout << COLOR_GREEN << "Request Headers: " << COLOR_RESET << std::endl;
		for (std::map<std::string, std::string>::const_iterator it = this->request_headers.begin(); it != this->request_headers.end(); ++it)
			std::cout << it->first << "=>" << it->second;
		std::cout << "****** CGI ----: " <<this->server_config.location.getCgiExe() << std::endl;
		std::cout << COLOR_GREEN << "Request Body: {{"  << this->request_body << "}}" << COLOR_RESET << std::endl;
	}
}

bool	Request::allowedURIchars(std::string& str)
{
	std::string allowedChars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-._~:/?#[]%@!$&'()*+,;= ";
	for (size_t i = 0; i < str.length(); i++)
	{
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
		std::string key = line.substr(0, pos + 1);
		if (key.find(" ") != std::string::npos || key.find(":") == std::string::npos || key.empty())
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
		if (key.compare("Connection:") == 0)//if the key is Connection
		{
			std::string tmp = value;
			stringTrim(tmp);
			this->_connection = tmp;
		}
		if (key.compare("Transfer-Encoding:") == 0)//if the key is Transfer-Encoding
		{
			if (value == " chunked\r\n")
				this->transfer_encoding = "chunked";
			else
				return (this->_status_code = "501 Not Implemented", true);
		}
		if (key.compare("Content-Type:") == 0)//if the key is Content-Type
			this->content_type = value;
		this->request_headers[key] = value;
	}
	return (true);
}

bool	Request::checkMethod()
{
	if (this->method.compare("GET") != 0
		&& this->method.compare("POST") != 0
		&& this->method.compare("DELETE") != 0
		)
		this->_status_code = "405 Method Not Allowed";
	return (true);
}

void       Request::handlePathInfo()
{
	if (this->path.find(".php") != std::string::npos || this->path.find(".py") != std::string::npos)
	{
		if (this->path.find(".php/") != std::string::npos)
		{
			if (this->path.find(".php/") + 5 < this->path.length())
			{
				this->server_config.path_info = this->path.substr(this->path.find(".php") + 4);
				this->path = this->path.substr(0, this->path.find(".php") + 4);
			}
		}
		else if (this->path.find(".py/") != std::string::npos)
		{
			if (this->path.find(".py/") + 4 < this->path.length())
			{
				this->server_config.path_info = this->path.substr(this->path.find(".py") + 3);
				this->path = this->path.substr(0, this->path.find(".py") + 3);
			}
		}
	}
}

void	Request::handleQueryString()
{
	if (this->path.find("#") != std::string::npos)
		this->path = this->path.substr(0, this->path.find("#"));
	if (this->path.find("?") != std::string::npos)
	{
		this->query_string = this->path.substr(this->path.find("?") + 1);
		this->path = this->path.substr(0, this->path.find("?"));
	}
}

bool 	Request::checkPath()
{
	if (this->path.find("..") != std::string::npos)
		return (_status_code = "403 Forbidden", true);
	this->server_config.request_url = this->path;
	this->handleQueryString();
	this->handlePathInfo();
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
			return (this->_has_headers = true, true);
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
	if (this->request_body.length() >= (this->content_length))
	{
		this->_has_body = true;
		return (true);
	}
	return (false);
}

// Function to extract and concatenate the chunks from a chunked request
bool 	Request::extractChunks(std::string& request)
{
    std::string delimiter = "\r\n";
    size_t pos = 0;
    std::string chunk;

    while ((pos = request.find(delimiter)) != std::string::npos)
    {
        //hex to decimal
        chunk = request.substr(0, pos);
        int decimal = 1;
        std::stringstream ss(chunk);
        ss >> std::hex >> decimal;
        if (chunk == "0")
        {
            this->request_body = this->_chunked_body;
            return true;
        }
        this->_chunked_body.append(request.substr(pos + 2, decimal));
        // Remove the chunk from the request
        request.erase(0, pos + 2 + decimal + 2);
    }
    return (false);
}

bool			Request::storeChunkedRequestBody()
{
	if (this->_chunk_size == -1)
	{
		size_t pos = this->request_body.find("\r\n");
		if (pos != std::string::npos)
		{
			std::stringstream ss;
			ss << std::hex << this->request_body.substr(0, pos);
			ss >> this->_chunk_size;
			this->request_body.erase(0, pos + 2);
		}
	}
	if (this->_chunk_size > 0 && (long long)this->request_body.length() >= this->_chunk_size)
	{
		_chunked_body.append(this->request_body.substr(0, this->_chunk_size));
		this->request_body.erase(0, this->_chunk_size + 2);
		this->_chunk_size = -1;
		return storeChunkedRequestBody();
	}
	if (this->_chunk_size == 0)
	{
		this->request_body = this->_chunked_body;
		this->read_bytes = this->request_body.length();
		this->_chunked_body.clear();
		this->_chunk_size = -1;
		this->_has_body = true;
		return (true);
	}
	return (false);
}

bool 		Request::checkEssentialHeaders(const std::map<std::string, std::string>& request_headers)
{
	std::map<std::string, std::string>::const_iterator it = request_headers.find("Host:");
	if (it == request_headers.end() || it->second.empty() || it->second == " \r\n")
	{
		return false;
	}
	return true;
}

bool   		Request::handleRequestBody()
{
	if (this->content_length > 0 || this->transfer_encoding == "chunked")
	{
		if (this->_body_size != -1 && (long long)this->request_body.length() > this->_body_size)
			return (this->_status_code = "413 Request Entity Too Large", true);
		else if (this->hasHeaders() && this->content_length > 0)
			return (this->storeRequestBody());
		else if (this->hasHeaders() && this->transfer_encoding == "chunked")
			return (this->storeChunkedRequestBody());
	}
	if ((this->hasHeaders() && this->content_length == 0 && this->transfer_encoding == "") || ! this->request_body.length())
		return (true);
	return (false);
}

bool	Request::receiveRequest()//must read the request
{
	int readStatus;
	memset(this->buffer, 0, MAX_REQUEST_SIZE + 1);
	readStatus = read(this->fd, this->buffer, MAX_REQUEST_SIZE);
	if (readStatus <= 0)
		return (this->read_bytes = 0, false);
	this->read_bytes += readStatus;
	this->request_body.append(this->buffer, readStatus);
	if ( ! this->hasHeaders())
	{
		size_t pos = this->request_body.find("\r\n\r\n");
		if (pos != std::string::npos)
		{
			this->request_string = this->request_body.substr(0, pos + 4);
			this->request_body = this->request_body.substr(pos + 4);
		}
		this->handleRequestHeaders(this->request_string);
		if (this->hasHeaders() && ! this->checkEssentialHeaders(this->request_headers))
			this->_status_code = "400 Bad Request";
		if (this->handleBadRequest() == false)
			return (true);
	}
	return (this->handleRequestBody());
}


bool   Request::sendResponse()
{
	return (Response::onPollout(this->fd));
}
