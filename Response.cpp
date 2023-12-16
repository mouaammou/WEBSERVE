/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 22:54:14 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/11 00:50:54 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <string>
# include <fstream>
# include <map>
# include <vector>
# include <stdexcept>
# include <sstream>
# include <unistd.h>
# include <ctime>
# include "StatusCodes.cpp"

typedef struct config
{
	std::string location;
	std::string root;
	std::string translated_path;
	std::string methods;

	std::string index; //main.py
	std::string type; //text/html
	std::string charset; //utf-8
	std::string language; //en

	bool		autoindex;
	std::string	autoindex_format;

	std::string cgi_extension;
	bool		is_a_cgi;

	bool		is_a_redirection;
	std::string	redirection_path;

	std::string getErrorPage( std::string code )
	{
		(void)code;
		return "/Users/samjaabo/Desktop/webserv/tmp/templates/error_page.html";
	}
	config( void )
	{
		location = "/";
		root = "/Users/samjaabo/Desktop/webserv/tmp/templates/";
		translated_path = "/Users/samjaabo/Desktop/webserv/tmp/templates/main.py";
		methods = "GET,POST,DELETE";

		index = "index.html"; //main.py
		type = "text/html"; //text/html
		charset = "utf-8"; //utf-8
		language = "en"; //en

		autoindex = true;
		autoindex_format = ".html";

		cgi_extension = ".py";
		is_a_cgi = false;

		is_a_redirection = false;
		redirection_path = "/home/";
	}

}	t_config;

class Response
{
	public:

	// <clinet_fd, response_content>
	static const StatusCodes	status_codes;
	static std::map<int, std::string>	ready_responses;
	static std::string getClientResponse( int client_fd )//empty string if not ready
	{
		std::map<int, std::string>::iterator it;
		it = ready_responses.find(client_fd);
		if (it == ready_responses.end())
			return "";
		std::string ret = it->second;
		ready_responses.erase(it);
		return ret;
	}

	private:

	t_config					config;

	std::string					file_content;
	std::stringstream			response_content;
	std::vector <std::string>	headers;

	bool readFile( std::string file )
	{
		std::ifstream sfile(file);
		if ( ! sfile.is_open())
			return false;
		std::string line;
		while (std::getline(sfile, line))
			file_content.append(line + "\r\n");
		if ( ! sfile.eof() && sfile.bad())
		{
			sfile.close();
			file_content.clear();
			return false;
		}
		sfile.close();
		return true;
	}

	void	generateStatusCodeResponse( std::string code, std::string path="" )
	{
		if ( code[0] == '4' || code[0] == '5') // 4xx, 5xx errors
		{
			config.type = "text/html";
			readFile(config.getErrorPage(code));
			std::cerr << "Error: " << config.getErrorPage(code) << std::endl;
		}
		else if ( ! path.empty() ) // 2xx requires a body
		{
			if ( ! readFile(path))
				return generateStatusCodeResponse("500");
		}
		response_content << status_codes.getStatusLine(code);
		response_content << "Cache-Control: no-store\r\n";
		response_content << "Server: Webserv/1.0\r\n";
		// response_content << "Date: " << std::ctime(new time_t(time(NULL)));
		// if (code[0] == '5' || code[0] == '4')
		// 	response_content << "Connection: close\r\n";
		// else
		// 	response_content << "Connection: keep-alive\r\n";
		for (size_t i=0; i < headers.size(); ++i)
			response_content << headers[i] << "\r\n";
		if ( ! file_content.empty() )
		{
			response_content << "Content-Length: " << file_content.length() << "\r\n";
			response_content << "Content-Type: " << config.type << "\r\n";
		}
		response_content << "\r\n";
		response_content << file_content;
		file_content.clear();
	}

	void addAdditionalStatusCodeHeaders( std::string code, std::string value="" )
	{
		if ((code[0] == '3' || code == "201") && code != "304")
			headers.push_back(std::string("Location: ") + (value.empty() ? config.location : value));
		else if ( code == "405")
			headers.push_back(std::string("Allow: ") + (value.empty() ? config.methods : value));
	}
	
	public:

	Response( t_config &config, int client_fd, std::string code, std::string value="" ): config(config)
	{
		addAdditionalStatusCodeHeaders(code, value);
		if (code == "200")
			generateStatusCodeResponse(code, config.translated_path);
		else
			generateStatusCodeResponse(code);
		ready_responses[client_fd] = response_content.str();
	}
};

std::map<int, std::string>	Response::ready_responses;
const StatusCodes Response::status_codes = StatusCodes();