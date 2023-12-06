/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/29 21:20:59 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/30 22:38:22 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HeaderFieldsParser.cpp"

//	https://www.rfc-editor.org/rfc/rfc7230#section-3.2
//	3.2.  Header Fields
//	Each header field consists of a case-insensitive field name followed
//	by a colon (":"), optional leading whitespace, the field value, and
//	optional trailing whitespace.

class ResponseTypes : public HeaderFieldsParser
{
	public:

	static const int DOCUMENT_RESPONSE = 1;
	static const int CLIENT_REDIRDOC_RESPONSE = 2;
	static const int LOCAL_REDIR_RESPONSE = 3;
	static const int CLIENT_REDIR_RESPONSE = 4;
	static const int INVALID_RESPONSE = 5;

	private:

	int	content_type_exists;
	int	status_exists;
	int	client_location_exists;
	int	local_location_exists;
	int	other_fields_exists;

	void	getAllFields( std::string token )
	{
		// Content-Type [ Status ] *other-field
		content_type_exists = 0;
		status_exists = 0;
		client_location_exists = 0;
		local_location_exists = 0;
		for (;token.length();)
		{
			size_t pos = token.find("\n"); // always "\n" exists
			std::string	line = token.substr(0, pos + 1);
			if ( isContentType(line) )
			{
				std::cout << "content" <<std::endl;
				++content_type_exists;
			}
			else if ( isStatus(line) )
			{
				std::cout << "status" <<std::endl;
				++status_exists;
			}
			else if ( isClientLocation(line) )
			{
				std::cout << "client" <<std::endl;
				++client_location_exists;
			}
			else if ( isLocalLocation(line) )
			{
				++local_location_exists;
				std::cout << "local" <<std::endl;
			}
			else
				++other_fields_exists;
			// std::cout << "$" << line <<"$";
			token = token.substr(pos + 1);
		}
		std::cout << "\ncon-t:" << content_type_exists;
		std::cout << "\nst-t:" << status_exists;
		std::cout << "\ncl-l:" << client_location_exists;
		std::cout << "\nlo-l:" << local_location_exists ;
		std::cout << "\not-f:" << other_fields_exists << std::endl;
	}

	public:

	int getResponseType( std::string token )
	{
		if ( ! isValidHeaders(token) )
			return INVALID_RESPONSE;
		size_t pos = token.find("\n\n");
		getAllFields(token.substr(0, pos + 1));
		// client-Location Status Content-Type *other-field NL
		if (client_location_exists && status_exists && content_type_exists)
			return CLIENT_REDIRDOC_RESPONSE;
		if (client_location_exists)
			return CLIENT_REDIR_RESPONSE;
		if (local_location_exists)
			return LOCAL_REDIR_RESPONSE;
		if (content_type_exists)
			return DOCUMENT_RESPONSE;
		return INVALID_RESPONSE;
	}
};

void trimOptionalWhiteSpaces( std::string &token )
{
	size_t i=0;
	for (; token[i] == ' ' || token[i] == '\t'; ++i);
	token = token.substr(i);
	if (token.length() < 2)
		return ;
	i=token.length() - 2; // "\0"
	for (; i && (token[i] == ' ' || token[i] == '\t'); --i);
	token = token.substr(0, i + 1);
	token += '\n';
	// std::cout<<"-->" << token << std::endl;
}


// static const int DOCUMENT_RESPONSE = 1;
// static const int CLIENT_REDIRDOC_RESPONSE = 2;
// static const int LOCAL_REDIR_RESPONSE = 3;
// static const int CLIENT_REDIR_RESPONSE = 4;
// static const int INVALID_RESPONSE = 5;
int main( void )
{
	std::string str;
	ResponseTypes p;

	str = "";
	str =	"Content-Type:text/html\n"
			"Content-Type:text/html\n"
			"Location: /local/redirect/ \n"
			"X-CGI-:   fdf  \n"
			"Status:   358  OK \n\n";

	int i = p.getResponseType(str);

	std::cout << "$" << i <<"$" << std::endl;

	// std::string t = "    fgfd dfgdf      \n";
	// trimOptionalWhiteSpaces(t);
	// std::cout << "$" << t <<"$" << std::endl;
}
