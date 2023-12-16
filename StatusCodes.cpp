/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodes.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo < samjaabo@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:23 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/14 18:04:26 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
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
		catch ( const std::out_of_range &e )
		{
			return "INVALID STATUS CODE getStatusCode()";
		}
	}
};
