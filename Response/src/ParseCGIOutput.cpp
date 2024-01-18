/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseCGIOutput.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 17:57:35 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/18 01:07:56 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ParseCGIOutput.hpp"

std::string ParseCGIOutput::getFiled( std::string field )
{
	std::string header = headers;
	for (;header.length();)
	{
		std::size_t pos = header.find("\n");
		std::string	line = header.substr(0, pos);
		std::size_t sep = line.find(":");
		if (line.substr(0, sep) == field && sep != std::string::npos)
		{
			std::size_t start = sep + 1;
			for (; start < line.length() && (line[start] == ' ' || line[start] == '\t'); ++start);
			std::size_t end = line.length() - 1;
			for (; end > start && (line[end] == ' ' || line[end] == '\t'); --end);
			return line.substr(start, end-start+1);
		}
		header = header.substr(pos + 1);
	}
	return "";
}

void ParseCGIOutput::translateHeaders( void )
{
	for (;headers.length();)
	{
		std::size_t pos = headers.find("\n");
		std::string	line = headers.substr(0, pos);
		std::size_t sep = line.find(":");
		char c = line[line.length() - 1];
		if (line.substr(0, sep) != "Status")
		{
			if (c == '\r')
				line.append("\n");
			else
				line.append("\r\n");
			new_headers.append(line);
		}
		headers.erase(0, pos + 1);
	}
}

size_t ParseCGIOutput::getContentLength( void )
{
	try
	{
		return std::stoull(getFiled("Content-Length"));
	}
	catch(const std::exception& e)
	{
	}
	return 0;
}

bool ParseCGIOutput::thereIsContentLength( void )
{
	return getFiled("Content-Length").empty() ? false : true;
}

void ParseCGIOutput::generateStatusLine( void )
{
	std::string status_code = getFiled("Status");
	if (status_code.empty())
	{
		if (getFiled("Location").empty())
			new_headers.append(StatusCodes().getStatusLine("200"));
		else
			new_headers.append(StatusCodes().getStatusLine("302"));
	}
	else
		new_headers.append(std::string("HTTP/1.1 ") + status_code + "\r\n");
}

void ParseCGIOutput::additionalHeaders( void )
{
	new_headers.append("Cache-Control: no-store\r\n");
	new_headers.append("Server: Webserv/1.0\r\n");
}

void	ParseCGIOutput::phpResponse( std::string &output, config &args )
{
	std::cout  << "<<<<<<<<<<<<<< phpResponse >>>>>>>>>>>>" << std::endl;
	size_t pos = output.find("\r\n\r\n");
	if (pos == std::string::npos)
	{
		args.response_code = "502";
		Response resp(args);
		return;
	}
	headers = output.substr(0, pos + 2);
	output.erase(0, pos + 4);
	generateStatusLine();
	std::stringstream str;
	str << "Content-Length: " << output.length() << "\r\n";
	new_headers.append(str.str());
	translateHeaders();
	additionalHeaders();
	new_headers.append("\r\n");
	SendResponse(new_headers + output, -1, args.socket_fd);
	
}

ParseCGIOutput::ParseCGIOutput(int status, std::string &output, config &args )
{
	if (args.response_code[0] == '5')
	{
		Response resp(args);
		return ;
	}
	size_t pos = args.translated_path.rfind(".php");
	if (pos != std::string::npos && pos == args.translated_path.length() - 4)
	{
		phpResponse(output, args);
		return ;
	}
	pos = output.find("\n\n");
	if (pos == std::string::npos)
	{
		args.response_code = "502";
		Response resp(args);
		return;
	}
	headers = output.substr(0, pos + 1);
	if (thereIsContentLength())
		body = output.substr(pos + 2, getContentLength());
	else
		body = output.substr(pos + 2);
	if (thereIsContentLength() && body.length() < getContentLength())
	{
		args.response_code = "502";
		Response resp(args);
		return;
	}
	else if ( ! thereIsContentLength() && status != 0)
	{
		args.response_code = "502";
		Response resp(args);
		return;
	}
	generateStatusLine();
	if ( ! thereIsContentLength() && body.length() > 0)
	{
		std::stringstream str;
		str << "Content-Length: " << body.length() << "\r\n";
		new_headers.append(str.str());
	}
	translateHeaders();
	additionalHeaders();
	new_headers.append("\r\n");
	SendResponse(new_headers + body, -1, args.socket_fd);
}
