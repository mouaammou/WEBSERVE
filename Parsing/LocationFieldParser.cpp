/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationFieldParser.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 19:31:07 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/30 22:18:32 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <cctype>
# include "ContentTypeFieldParser.cpp"

class LocationFieldParser : virtual ContentTypeFieldParser // mabe virtual
{
	private:

	bool isSchemeChar( int c )
	{
		// alpha *( alpha | digit | "+" | "-" | "." )
		return std::isalnum(c) || c=='+' || c=='-' || c=='.';
	}

	bool isSigmentChar( int c )
	{
		// unreserved | escaped | extra
		// i cant handle escaped here because: "%" hex hex
		return std::isalnum(c) || c==':' || c=='@' || c=='&' || c=='=' || c=='+' || c=='$' || c==',' ||
			c=='-' || c=='_' || c=='.' || c=='!' || c=='~' || c=='*' || c=='\'' || c=='(' || c==')'; //extra
	}

	bool isSigmentString( std::string token )
	{
		for (size_t i=0; i < token.length(); ++i)
		{
			if (token[i] == '%')// escaped "%" hex hex
			{
				if ( ! token[i + 1] || ! token[i + 2])
					return false;
				if ( ! std::isxdigit(token[i + 1]) || ! std::isxdigit(token[i + 2]))
					return false;
				i += 3;
			}
			else if ( ! isSigmentChar(token[i]) )
				return false;
		}
		return true;
	}

	bool isSchemeString( std::string token )
	{
		// 1*( alpha *( alpha | digit | "+" | "-" | "." ) )
		if (token.length() == 0)
			return false;
		if ( ! std::isalpha(token[0]) )
			return false;
		for (size_t i=0; i < token.length(); ++i)
		{
			if ( ! isSchemeChar(token[i]) )
				return false ;
		}
		return true;
	}

	// path-segments   = segment *( "/" segment )

	bool isPathSigments( std::string token )
	{
		// segment *( "/" segment )
		if ( token[0] == '/') // should not be '/'
			return false;
		size_t pos = token.find('/');
		if ( ! isSigmentString(token.substr(0, pos)) )
			return false;
		if (pos == std::string::npos)
			return true;
		for (;pos != std::string::npos;)
		{
			token = token.substr(pos + 1); // from "/" to "\0"
			if (token.length() == 0) // "/" should be folowed by a sigment
				return false;
			pos = token.find('/');
			if ( ! isSigmentString(token.substr(0, pos)) ) // from "/" to "/"
				return false;
		}
		return true;
	}

	bool isAbsPath( std::string token )
	{
		// segment *( "/" segment )
		if ( token[0] != '/') // should be '/'
			return false;
		return isPathSigments(token.substr(1));
	}
	/// @brief //////
	/// @return

	// absoluteURI   = scheme ":" ( hier_part | opaque_part )
	// hier_part     = ( net_path | abs_path ) [ "?" query ]
	// net_path      = "//" authority [ abs_path ]
	// abs_path      = "/"  path_segments

	// authority     = server | reg_name

	bool isRegNameChar( int c )
	{
		// unreserved | escaped | "$" | "," | ";" | ":" | "@" | "&" | "=" | "+"
		// i cant handle escaped here because: "%" hex hex
		return std::isalnum(c) || c=='-' || c=='_' || c=='.' || c=='!' || c=='~' || c=='*' || c=='\'' || c=='(' || c==')' //unreserved
			|| c=='$' || c==',' || c==';' || c==':' || c=='@' || c=='&' || c=='=' || c=='+';
	}

	bool isUserinfoChar( int c )
	{
		// unreserved | escaped | "$" | "," | ";" | ":" | "@" | "&" | "=" | "+"
		// i cant handle escaped here because: "%" hex hex
		return std::isalnum(c) || c=='-' || c=='_' || c=='.' || c=='!' || c=='~' || c=='*' || c=='\'' || c=='(' || c==')' //unreserved
			|| c=='$' || c==',' || c==';' || c==':' || c=='&' || c=='=' || c=='+';
	}

	bool isRegNameString( std::string token )
	{
		for (size_t i=0; i < token.length(); ++i)
		{
			if (token[i] == '%')// escaped "%" hex hex
			{
				if ( ! token[i + 1] || ! token[i + 2])
					return false;
				if ( ! std::isxdigit(token[i + 1]) || ! std::isxdigit(token[i + 2]))
					return false;
				i += 3;
			}
			else if ( ! isRegNameChar(token[i]) )
				return false;
		}
		return true;
	}

	bool isDigitString( std::string token )
	{
		if (token.length() == 0)
			return false;
		for (size_t i=0; i < token.length(); ++i)
		{
			if ( ! std::isdigit(token[i]) )
				return false;
		}
		return true;
	}

	bool isUserinfoString( std::string token )
	{
		for (size_t i=0; i < token.length(); ++i)
		{
			if (token[i] == '%')// escaped "%" hex hex
			{
				if ( ! token[i + 1] || ! token[i + 2])
					return false;
				if ( ! std::isxdigit(token[i + 1]) || ! std::isxdigit(token[i + 2]))
					return false;
				i += 3;
			}
			else if ( ! isUserinfoChar(token[i]) )
				return false;
		}
		return true;
	}

	// char
	// domainlabel =  alphanum | alphanum *( alphanum | "-" ) alphanum

	bool isDomainlabel( std::string token )
	{
		//  alphanum | alphanum *( alphanum | "-" ) alphanum
		if (token.length() == 0)
			return false;
		if ( ! std::isalnum(token[0]) )
			return false;
		size_t i=1;
		for (; i < token.length() - 1; ++i)
		{
			if ( ! std::isalnum(token[i]) && token[i] != '-')
				return false;
		}
		if ( ! std::isalnum(token[i]) && token.length() != 1)
			return false;
		return true;
	}

	bool isToplabel( std::string token )
	{
		//  alpha | alpha *( alphanum | "-" ) alphanum
		if (token.length() == 0)
			return false;
		if ( ! std::isalpha(token[0]) )
			return false;
		size_t i=1;
		for (; i < token.length() - 1; ++i)
		{
			if ( ! std::isalnum(token[i]) && token[i] != '-')
				return false;
		}
		if ( ! std::isalnum(token[i]) && token.length() != 1)
			return false;
		return true;
	}

	bool isHostname( std::string token )
	{
		// *( domainlabel "." ) toplabel [ "." ]
		// if ( token[0] == '.') // should not be '/'
		// 	return false;
		if (token[token.length() - 1] == '.')
			token = token.substr(0, token.length() - 1);
		size_t pos = 0;
		for (;;)
		{
			token = token.substr(pos); // from "." to "\0"
			pos = token.find('.');
			if ( pos == std::string::npos )
				break ;
			// std::cout <<token.substr(0, pos)<<std::endl;
			if ( ! isDomainlabel(token.substr(0, pos)) ) // from "." to "."
				return false;
			++pos;
		}
		if (isToplabel(token))
			return true;
		return false;
	}

	bool isIPv4address( std::string token )
	{
		// 1*digit "." 1*digit "." 1*digit "." 1*digit
		for (size_t i=0; i < 3; ++i)
		{
			size_t pos = token.find('.');
			// std::cout <<token.substr(0, pos)<<std::endl;
			if (pos == std::string::npos)
				return false;
			// pos = token.find('.');
			// if ( pos == std::string::npos )
			// 	break ;
			if ( ! isDigitString(token.substr(0, pos)) ) // from "." to "."
				return false;
			token = token.substr(pos + 1); // from "." to "\0"
		}
		if ( ! isDigitString(token) ) // last one
			return false;
		return true;
	}

	bool isHost( std::string token )
	{
		return isHostname(token) || isIPv4address(token);
	}

	bool isHostport( std::string token )
	{
		// host [ ":" *digit ]
		size_t pos = token.find(':');
		if ( ! isHost(token.substr(0, pos)))
			return false;
		if ( pos == std::string::npos)
			return true;
		token = token.substr(pos + 1);
		if (token.length() == 0)
			return true;
		return isDigitString(token);
	}
	bool isServer( std::string token )
	{
		// [ [ userinfo "@" ] hostport ]
		if (token.length() == 0) // for [*]
			return true;
		size_t pos = token.find('@');
		// std::cout << "#" << token.substr(0, pos)<<"#" << std::endl;
		if (pos != std::string::npos && ! isUserinfoString(token.substr(0, pos))) // userinfo
			return false;
		if ( pos == std::string::npos)
			return true;
		// std::cout <<"#"<<  token.substr(pos+1)<<"#" << std::endl;
		// token = token.substr(pos + 1);
		return isHostport(token.substr(pos + 1));
	}

	bool isAuthority( std::string token )
	{
		// server | reg_name
		return isServer(token) || isRegNameString(token);
	}

	bool isNetPath( std::string token ) //check test it again
	{
		// "//" authority  [ abs_path ]
		if (token.substr(0, 2) != "//")
			return false;
		token = token.substr(2);
		// if (token.length() == 0) // for [*]
		// 	return true;
		size_t pos = token.find('/'); // aps_path always start with "/"
		// std::cout << "#" << token.substr(0, pos)<<"#" << std::endl;
		if (pos == std::string::npos) // userinfo
		{
			if (isAuthority(token.substr(0, pos)))
				return true;
			return false;
		}
		if ( ! isAuthority(token.substr(0, pos)))
			return false;
		// std::cout <<"#"<<  token.substr(pos)<<"#" << std::endl;
		// token = token.substr(pos + 1);
		return isAbsPath(token.substr(pos));
	}

	bool isUricChar( int c )
	{
		// reserved | unreserved | escaped
		// i cant handle escaped here because: "%" hex hex
		return std::isalnum(c) || c=='-' || c=='_' || c=='.' || c=='!' || c=='~' || c=='*' || c=='\'' || c=='(' || c==')' //unreserved
			|| c==';' || c=='/' || c=='?' || c==':' || c=='@' || c=='&' || c=='=' || c=='+' || c=='$' || c==','; //reserved
	}

	bool isUricNoSlashChar( int c )
	{
		// reserved | unreserved | escaped
		// i cant handle escaped here because: "%" hex hex
		return std::isalnum(c) || c=='-' || c=='_' || c=='.' || c=='!' || c=='~' || c=='*' || c=='\'' || c=='(' || c==')' //unreserved
			|| c==';' || c=='?' || c==':' || c=='@' || c=='&' || c=='=' || c=='+' || c=='$' || c==','; //reserved
	}

	bool isUricString( std::string token )
	{
		for (size_t i=0; i < token.length(); ++i)
		{
			if (token[i] == '%')// escaped "%" hex hex
			{
				if ( ! token[i + 1] || ! token[i + 2])
					return false;
				if ( ! std::isxdigit(token[i + 1]) || ! std::isxdigit(token[i + 2]))
					return false;
				i += 3;
			}
			else if ( ! isUricChar( token[i]) )
				return false;
		}
		return true;
	}

	bool isHierPart( std::string token )
	{
		// ( net_path | abs_path ) [ "?" *uric ]
		// if (token.substr(0, 2) != "?")
		// 	return false;
		size_t pos = token.find('?'); // aps_path always start with "/"
		// token = token.substr(2);
		if ( ! isNetPath(token.substr(0, pos)) && ! isAbsPath(token.substr(0, pos)))
			return false;
		if ( pos == std::string::npos )
			return true;
		return isUricString(token.substr(pos + 1));
	}

	bool isOpaquePart( std::string token )
	{
		// uric_no_slash *uric
		if ( ! isUricNoSlashChar(token[0]))
			return false;
		return isUricString(token.substr(1));
	}

	bool isAbsoluteUri( std::string token )
	{
		// scheme ":" ( hier_part | opaque_part )
		size_t pos = token.find(':'); // aps_path always start with "/"
		if (pos == std::string::npos)
			return false;
		// token = token.substr(2);
		if ( ! isSchemeString(token.substr(0, pos)))
			return false;
		token = token.substr(pos + 1);
		return isHierPart( token ) || isOpaquePart( token );
	}

	bool isFragmentUri( std::string token )
	{
		// absoluteURI [ "#" *uric ]
		size_t pos = token.rfind('#');
		// token = token.substr(2);
		if ( ! isAbsoluteUri(token.substr(0, pos)))
			return false;
		if (pos == std::string::npos)
			return true;
		return isUricString(token.substr(pos + 1));
	}

	bool isLocalPathquery( std::string token )
	{
		// abs-path [ "?" *uric ]
		size_t pos = token.find('?'); // aps_path always start with "/"
		// token = token.substr(2);
		if ( ! isAbsPath(token.substr(0, pos)))
			return false;
		if ( pos == std::string::npos )
			return true;
		return isUricString(token.substr(pos + 1));
	}

	public:

	bool isClientLocation( std::string token )
	{
		// Location is case-sensitive
		// "Location:" fragment-URI NL
		if (token.length() < 10)
			return false;
		if (token[token.length() - 1] != '\n') //NL
			return false;
		token = token.substr(0, token.length() - 1); // del NL
		size_t pos = token.find(':');
		if (pos == std::string::npos)
			return false;
		// token = token.substr(2);
		// std::cout << token.substr(0, pos)<< std::endl;

		if ( token.substr(0, pos + 1) != "Location:" )
			return false;
		token = token.substr(pos + 1);
		trimOptionalWhiteSpaces(token);
		// std::cout <<"]]"<< token <<"[["<< std::endl;
		return isFragmentUri(token);
	}

	bool isLocalLocation( std::string token )
	{
		// Location is case-sensitive
		// "Location:" local-pathquery NL
		if (token.length() < 10)
			return false;
		if (token[token.length() - 1] != '\n') //NL
			return false;
		token = token.substr(0, token.length() - 1);//del NL
		size_t pos = token.find(':');
		if (pos == std::string::npos)
			return false;
		// token = token.substr(2);
		// std::cout << token.substr(0, pos)<< std::endl;

		if ( token.substr(0, pos + 1) != "Location:" )
			return false;
		token = token.substr(pos + 1);
		trimOptionalWhiteSpaces(token);
		// token += '\n';
		// std::cout <<"]]"<< token <<"[["<< std::endl;
		return isLocalPathquery(token);
	}

	bool isLocation( std::string token )
	{
		// local-Location | client-Location
		return isLocalLocation(token) || isClientLocation(token);
	}

};