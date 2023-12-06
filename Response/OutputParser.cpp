/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 18:51:54 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/04 20:43:53 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "OutputParser.hpp"

std::size_t OutputParser::buffsize = 32000;

std::string OutputParser::getLocalLocation( std::string header )
{
	for (;header.length();)
	{
		std::size_t pos = header.find("\n");
		std::string	line = header.substr(0, pos);
		std::size_t sep = header.find(":");
		if (line.substr(0, sep) == "Location" && sep != std::string::npos)
		{
			std::size_t start = sep + 1;
			for (; start < line.length() && (line[start] == ' ' || line[start] == '\t'); ++start);
			std::size_t end = line.length() - 1;
			for (; end > start && (line[end] == ' ' || line[end] == '\t'); --end);
			line = line.substr(start, end-start+1);
			if ( line[0] =='/')
				return line;
			break ;
		}
		header = header.substr(pos + 1);
	}
	return "";
}

std::size_t OutputParser::getContentLength( std::string header )
{
	for (;header.length();)
	{
		std::size_t pos = header.find("\n");
		std::string	line = header.substr(0, pos);
		std::size_t sep = header.find(":");
		if (line.substr(0, sep) == "Content-Length" && sep != std::string::npos)
		{
			std::size_t start = sep + 1;
			for (; start < line.length() && (line[start] == ' ' || line[start] == '\t'); ++start);
			std::size_t end = line.length() - 1;
			for (; end > start && (line[end] == ' ' || line[end] == '\t'); --end);
			return static_cast<std::size_t>((std::atoll(line.substr(start, end-start+1).c_str())));
		}
		header = header.substr(pos + 1);
	}
	return UINT64_MAX;
}

bool OutputParser::thereIsContentLength( std::string header )
{
	for (;header.length();)
	{
		std::size_t pos = header.find("\n");
		std::string	line = header.substr(0, pos);
		std::size_t sep = header.find(":");
		if (line.substr(0, sep) == "Content-Length" && sep != std::string::npos)
			return true;
		header = header.substr(pos + 1);
	}
	return false;
}

std::string OutputParser::getStatusLine( std::string header )
{
	for (;header.length();)
	{
		std::size_t pos = header.find("\n");
		std::string	line = header.substr(0, pos);
		std::size_t sep = header.find(":");
		if (line.substr(0, sep) == "Status" && sep != std::string::npos)
		{
			std::string	http = "HTTP/1.1 ";
			std::size_t start = sep + 1;
			for (; start < line.length() && (line[start] == ' ' || line[start] == '\t'); ++start);
			std::size_t end = line.length() - 1;
			for (; end > start && (line[end] == ' ' || line[end] == '\t'); --end);
			return http + line.substr(start, end-start+1) + "\r\n";
		}
		header = header.substr(pos + 1);
	}
	return "HTTP/1.1 200 OK\r\n";
}

