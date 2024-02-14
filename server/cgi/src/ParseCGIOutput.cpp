/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseCGIOutput.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 17:57:35 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/26 15:11:03 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ParseCGIOutput.hpp"

std::string ParseCGIOutput::getFiled(std::string field)
{
	std::string header = headers;
	for (; header.length();)
	{
		std::size_t pos = header.find("\n");
		std::string line = header.substr(0, pos);
		std::size_t sep = line.find(":");
		if (line.substr(0, sep) == field && sep != std::string::npos)
		{
			std::size_t start = sep + 1;
			for (; start < line.length() && (line[start] == ' ' || line[start] == '\t'); ++start)
				;
			std::size_t end = line.length() - 1;
			for (; end > start && (line[end] == ' ' || line[end] == '\t'); --end)
				;
			return line.substr(start, end - start + 1);
		}
		header = header.substr(pos + 1);
	}
	return "";
}

void ParseCGIOutput::translateHeaders(void)
{
	for (; headers.length();)
	{
		std::size_t pos = headers.find("\n");
		std::string line = headers.substr(0, pos);
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

void ParseCGIOutput::generateStatusLine(void)
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
	std::cout << "        Status:   [" << new_headers;
}

void ParseCGIOutput::additionalHeaders(void)
{
	size_t pos1 = headers.find("Date:");
	size_t pos2 = headers.find("\r\nDate:");
	if (pos2 == std::string::npos && pos1 != 0)
		new_headers.append("Date: ").append(Response::getDate()).append("\r\n");
	pos1 = headers.find("Server:");
	pos2 = headers.find("\r\nServer:");
	if (pos2 == std::string::npos && pos1 != 0)
		new_headers.append("Server: Webserv/1.0\r\n");
}

ParseCGIOutput::ParseCGIOutput(int status, std::string &output, config &args)
{
	if (args.response_code[0] == '5')
	{
		Response resp(args);
		return;
	}
	if (status == SIGKILL)
	{
		args.response_code = "504";
		Response resp(args);
		return;
	}
	size_t pos = output.find("\r\n\r\n");
	if ((args.translated_path.substr(args.translated_path.length() - 3) == ".py" && status != 0) || (status != 0 && status != 255) || pos == std::string::npos)
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
	additionalHeaders();
	translateHeaders();
	new_headers.append("\r\n");
	SendResponse(new_headers + output, -1, args.socket_fd);
}
