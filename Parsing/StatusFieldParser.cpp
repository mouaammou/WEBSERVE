/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusFieldParser.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:51:20 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/30 22:30:06 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <cctype>
# include "ContentTypeFieldParser.cpp"

class StatusFieldParser : virtual ContentTypeFieldParser
{
	private:

	bool isPrintableString( std::string token )
	{
		// any printable char
		size_t length = token.length();
		for (size_t i=0; i < length; ++i)
		{
			if ( ! std::isprint(token[i]) )
				return false;
		}
		return true;
	}

	public:

	bool isStatus( std::string token )
	{
		// Status is case-sensitive
		// "Status:" 3digit SP isprint NL
		if (token.length() < 12)
			return false;
		if (token[token.length() - 1] != '\n')
			return false;
		token = token.substr(0, token.length() - 1);
		size_t pos = token.find(':');
		if (pos == std::string::npos)
			return false;
		if (token.substr(0, pos + 1) != "Status:")
			return false;
		token = token.substr(pos + 1);
		trimOptionalWhiteSpaces(token);
		// std::cout << "llll" << token[pos + 3] <<"llllll";
		if ( ! (std::isdigit(token[0]) && std::isdigit(token[1]) && std::isdigit(token[2])))
			return false;
		if (token[3] != ' ')
			return false;
		// std::cout <<  token.substr(pos + 4)<< std::endl;
		token = token.substr(4);
		// std::cout <<"]]"<< token <<"[["<< std::endl;
		return isPrintableString(token);
	}
};
