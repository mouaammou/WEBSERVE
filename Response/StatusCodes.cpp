/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodes.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 19:23:46 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/01 20:55:33 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "StatusCodes.hpp"



StatusCodes::StatusCodes( void )
{
	for (size_t i=0; i < sizeof(_scodes)/sizeof(_scodes[0]); ++i)
		codes[ _scodes[i][0] ] = _scodes[i][1];
}

std::string StatusCodes::getStatusLine( std::string code )
{
	// Status-Line = HTTP-Version SP Status-Code SP Reason-Phrase CRLF
	std::string h = "HTTP/1.1 ";
	if (codes.find(code) == codes.end())
		return "HTTP/1.1 200 OK\n";
	return h + code + " " + codes[code] + "\r\n";
}

std::string StatusCodes::_scodes[40][2] = {
	{ "100" , "Continue" },
	{ "101" , "Switching Protocols" },

	{ "200" , "OK" },
	{ "201" , "Created" },
	{ "202" , "Accepted" },
	{ "203" , "Non-Authoritative Information" },
	{ "204" , "No Content" },
	{ "205" , "Reset Content" },
	{ "206" , "Partial Content" },

	{ "300" , "Multiple Choices" },
	{ "301" , "Moved Permanently" },
	{ "302" , "Found" },
	{ "303" , "See Other" },
	{ "304" , "Not Modified" },
	{ "305" , "Use Proxy" },
	{ "306" , "Unused" },
	{ "307" , "Temporary Redirect" },

	{ "400" , "Bad Request" },
	{ "401" , "Unauthorized" },
	{ "402" , "Payment Required" },
	{ "403" , "Forbidde" },
	{ "404" , "Not Found" },
	{ "405" , "Method Not Allowed" },
	{ "406" , "Not Acceptable" },
	{ "407" , "Proxy Authentication Required" },
	{ "408" , "Request Timeout" },
	{ "409" , "Conflict" },
	{ "410" , "Gone" },
	{ "411" , "Lengh Required" },
	{ "412" , "Precondition Failed" },
	{ "413" , "Reqeust Entity Too Large" },
	{ "415" , "Unsupported Media Type" },
	{ "416" , "Request Range Not Satisfiable" },
	{ "417" , "Expectation Failed" }, // fails to recognise expect: Header field

	{ "500" , "Internal Server Error" },
	{ "501" , "Not Implemented" },
	{ "502" , "Bad Gateway" },
	{ "503" , "Service Unavailable" },
	{ "504" , "Gateway Timeout" },
	{ "505" , "HTTP Version Not Supported" },
};