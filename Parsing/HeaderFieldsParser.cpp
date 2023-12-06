/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HeaderFieldsParser.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 21:19:36 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/30 22:24:59 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include "StatusFieldParser.cpp"
# include "ContentTypeFieldParser.cpp"
# include "LocationFieldParser.cpp"

class HeaderFieldsParser: virtual protected ContentTypeFieldParser, protected LocationFieldParser, public StatusFieldParser
{
	private:

	bool isFieldChar( int c )
	{
		// any char except control chars, but space, newline and tab are allowed
		return c=='\t' || c=='\n' || !std::iscntrl(c);
	}

	bool isFieldValue( std::string token )
	{
		for (size_t i=0; i < token.length(); ++i)
		{
			if ( ! isFieldChar(token[i]) )
				return false;
		}
		return true;
	}

	bool isOtherField( std::string token )
	{
		// token ":" [ field-value ] NL
		if (token.length() < 3)
			return false;
		if (token[token.length() - 1] != '\n')
			return false;
		token = token.substr(0, token.length() - 1);
		size_t pos = token.find(':');
		if (pos == std::string::npos)
			return false;
		if ( ! isTokenString(token.substr(0, pos)))
			return false;
		//from ":" to "\0"
		// std::cout <<  token.substr(pos+1) << std::endl;
		return isFieldValue(token.substr(pos+1));
	}

	bool isCgiField( std::string token )
	{
		// Content-Type | Location | Status
		return isContentType(token) || isLocation(token) || isStatus(token);
	}

	bool isHeaderField( std::string token )
	{
		// CGI-field | other-field
		return isCgiField(token) || isOtherField(token);
	}

	public:

	bool isValidHeaders( std::string token )
	{
		// generic-response = 1*header-field NL [ response-body ]
		size_t pos = token.find("\n\n");
		if ( pos == std::string::npos )
			return false;
		token = token.substr(0, pos + 1);
		if ( token.length() == 0 )
			return false;
		for (;token.length();)
		{
			pos = token.find("\n");
			if ( ! isHeaderField(token.substr(0, pos + 1)) )
				return false;
			// std::cout << token.substr(0, pos + 1) ;
			token = token.substr(pos + 1);
		}
		return true;
	}
};