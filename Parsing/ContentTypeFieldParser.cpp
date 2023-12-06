/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ContentTypeFieldParser.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 15:13:45 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/30 22:13:46 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <cctype>

class ContentTypeFieldParser
{
	protected:

	void trimOptionalWhiteSpaces( std::string &token )
	{
		size_t i=0;
		for (; token[i] == ' ' || token[i] == '\t'; ++i);
		token = token.substr(i);
		if (token.length() < 2)
			return ;
		i=token.length() - 1; // "\0"
		for (; i && (token[i] == ' ' || token[i] == '\t'); --i);
		token = token.substr(0, i + 1);
		// token += '\n';
		// std::cout<<"-->" << token << std::endl;
	}

	bool isQuotedString( std::string token )
	{
		// <"> *qdtext <">
		// std::cout<<"-->" << token << std::endl;
		size_t length = token.length();
		if (length <= 1)
			return false ;
		if (token[0] != '"' && token[length - 1] != '"')
			return false;
		for (size_t i=0; i < length; ++i)
		{
			if ( ! isQdtextChar(token[i]) )
				return false;
		}
		return true;
	}

	bool isTokenChar( int c )
	{
		// 1*<any CHAR except CTLs or separators>
		return ! (std::iscntrl(c) || '('==c || ')'==c || '<'==c || '>'==c || '@'==c || ','==c || ';'==c || ':'==c ||
			'\\'==c || '"'==c || '/'==c || '['==c || ']'==c || '?'==c || '='==c || '{'==c ||
				'}'==c || ' '==c || '\t'==c);
	}

	bool isTokenString( std::string token )
	{
		// at least token should include 1 char
		// std::cout<<"-->" << token << std::endl;
		size_t length = token.length();
		if (length == 0)
			return false;
		for (size_t i=0; i < length; ++i)
		{
			if ( ! isTokenChar(token[i]) )
				return false;
		}
		return true;
	}

	private:

	bool isQdtextChar( int c )
	{
		// <any CHAR except <"> and CTLs but including LWSP>
		return c=='\t' || c=='\n' || !std::iscntrl(c) || c!='"';
	}

	bool isMediaType( std::string token )
	{
		// token "/" token *(";" (token "=" token|qouted-string) )
		// std::cout << token <<std::endl;
		if ( token.length() < 3 )
			return false;
		// token "/" token
		size_t pos = token.find( '/' );
		if ( pos == std::string::npos )
			return false;
		if ( ! isTokenString( token.substr( 0, pos ) ) ) // type
			return false;
		size_t end = token.find( ';');
		if ( ! isTokenString( token.substr( pos + 1, end-pos-1 ) ) ) // subtype
			return false;
		if ( end == std::string::npos )
			return true;
		// *(";" (token "=" token|qouted-string) )
		for (; end != std::string::npos;)
		{
			token = token.substr( end );// delete old evaluated token
			if (token[0] != ';')
				return false;
			end = token.find(';', 1);
			std::string query = token.substr(1, end - 1); // from ";" to ";" | key = value

			pos = query.find('='); // to separate key and value
			if (pos == std::string::npos) // "=" is mandatory
				return false;
			std::string key = query.substr(0, pos);
			std::string value = query.substr(pos + 1); // +1 dont include "="

			// std::cout<<"key-->" << key << std::endl;

			if ( ! isTokenString(key) )
				return false;
			// std::cout<<"value-->" << value << std::endl;
			if ( !isTokenString(value) && !isQuotedString(value) )
				return false;
		}
		return true;
	}

	public:

	bool isContentType( std::string token )
	{
		// "Content-Type:" media-type NL
		if (token.length() <= 15)
			return false;
		if (token[token.length() - 1] != '\n')
			return false;
		size_t pos = token.find(':');
		if (pos == std::string::npos)
			return false;
		if (token.substr(0, pos) != "Content-Type")
			return false;
		//from ":"] to ["\n"
		token = token.substr(pos + 1, token.length() - 2 - pos);
		trimOptionalWhiteSpaces(token);
		return isMediaType(token);
	}
};