/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 22:54:14 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/11 20:11:28 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
# include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <ctime>

class StatusCodes
{
	private:
	
	std::map<std::string, std::string>	codes;

	public:

	StatusCodes(void)
	{
		codes["100"] = "Continue";
		codes["101"] = "Switching Protocols";

		codes["200"] = "OK";
		codes["201"] = "Created";
		codes["202"] = "Accepted";
		codes["203"] = "Non-Authoritative Information";
		codes["204"] = "No Content";
		codes["205"] = "Reset Content";
		codes["206"] = "Partial Content";

		codes["300"] = "Multiple Choices";
		codes["301"] = "Moved Permanently";
		codes["302"] = "Found";
		codes["303"] = "See Other";
		codes["304"] = "Not Modified";
		codes["305"] = "Use Proxy";
		codes["306"] = "Unused";
		codes["307"] = "Temporary Redirect";

		codes["400"] = "Bad Request";
		codes["401"] = "Unauthorized";
		codes["402"] = "Payment Required";
		codes["403"] = "Forbidde";
		codes["404"] = "Not Found";
		codes["405"] = "Method Not Allowed";
		codes["406"] = "Not Acceptable";
		codes["407"] = "Proxy Authentication Required";
		codes["408"] = "Request Timeout";
		codes["409"] = "Conflict";
		codes["410"] = "Gone";
		codes["411"] = "Lengh Required";
		codes["412"] = "Precondition Failed";
		codes["413"] = "Reqeust Entity Too Large";
		codes["415"] = "Unsupported Media Type";
		codes["416"] = "Request Range Not Satisfiable";
		codes["417"] = "Expectation Failed";

		codes["500"] = "Internal Server Error";
		codes["501"] = "Not Implemented";
		codes["502"] = "Bad Gateway";
		codes["503"] = "Service Unavailable";
		codes["504"] = "Gateway Timeout";
		codes["505"] = "HTTP Version Not Supported";
	}

	const std::string getStatusLine( std::string const &code ) const
	{
		try
		{
			return std::string("HTTP/1.1 ") + code + std::string(" ") + codes.at(code) + "\r\n";
		}
		catch (const std::out_of_range &e)
		{
			return "INVALID STATUS CODE (from getStatusCode())";
		}
	}
};

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
		return root + "/error_page.html";
	}
	config( void )
	{
		location = "/";
		root = "/Users/samjaabo/Desktop/tmp/templates";
		translated_path = "html/index.html";
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
	protected:

	static const StatusCodes	status_codes;
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
		}
		else if ( ! path.empty() ) // 2xx requires a body
		{
			if ( ! readFile(path))
				return generateStatusCodeResponse("500");
		}
		response_content << status_codes.getStatusLine(code);
		response_content << "Cache-Control: no-store\r\n";
		response_content << "Server: Webserv/1.0\r\n";
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

	Response( t_config &config ): config(config)
	{

	}

	const std::string getResponseContent( std::string code, std::string value="")
	{
		addAdditionalStatusCodeHeaders(code, value);
		if (code == "200")
			generateStatusCodeResponse(code, config.translated_path);
		else
			generateStatusCodeResponse(code);
		return response_content.str();
	}
};

const StatusCodes Response::status_codes = StatusCodes();

int main(int ac, char **av)
{
	(void)ac;
	t_config config;

	std::cout << Response(config).getResponseContent(av[1]) << std::endl;
	return 0;
}
