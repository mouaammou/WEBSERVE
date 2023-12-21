/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseCGIOutput.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 17:57:35 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/21 20:41:41 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include "StatusCodes.cpp"
# include "Response.cpp"

class ParseCGIOutput
{
	private:

	std::string new_headers;
	std::string headers;
	std::string body;

	std::string getFiled( std::string field )
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

	void translateHeaders( void )
	{
		for (;headers.length();)
		{
			std::size_t pos = headers.find("\n");
			std::string	line = headers.substr(0, pos);
			std::size_t sep = line.find(":");
			if (line.substr(0, sep) != "Status")
			{
				new_headers.append(line + "\r\n");
			}
			headers = headers.substr(pos + 1);
		}
	}

	size_t getContentLength( void )
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

	bool thereIsContentLength( void )
	{
		return getFiled("Content-Length").empty() ? false : true;
	}

	void generateStatusLine( void )
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

	void additionalHeaders( void )
	{
		new_headers.append("Cache-Control: no-store\r\n");
		new_headers.append("Server: Webserv/1.0\r\n");
	}

	public:

	void response( int status, std::string output, int client_fd, t_config &config )
	{
		// std::cout << "output: " << output << std::endl;
		std::size_t pos = output.find("\n\n");
		if (pos == std::string::npos)
		{
			//found no headers
			Response(config, client_fd, "500");
			return;
		}
		headers = output.substr(0, pos + 1);
		if (thereIsContentLength())
			body = output.substr(pos + 2, getContentLength());
		else
			body = output.substr(pos + 2);
		if (thereIsContentLength() && body.length() < getContentLength())
		{
			//headers is case sensitve
			//Content-Length is not Content-length
			Response(config, client_fd, "500");
			return;
		}
		else if ( ! thereIsContentLength() && status != 0)
		{
			Response(config, client_fd, "500");
			return;
		}
		generateStatusLine();
		if ( ! thereIsContentLength() && body.length() > 0)
		{
			std::stringstream str;
			str << "Content-Length: " << body.length() << "\r\n";
			new_headers.append(str.str());
		}
		additionalHeaders();
		translateHeaders();
		new_headers.append("\r\n");
		// std::cout << "new_headers: " << new_headers << std::endl;
		// std::cout << "body: " << body << std::endl;
		Response::ready_responses[client_fd] = new_headers + body;
	}
};