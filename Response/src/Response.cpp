/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:22:50 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/30 11:13:14 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

void Response::runCGI( void )
{
	CGI::build( args );
}

void Response::autoIndex( void )
{
	if ( ! args.autoindex.compare("off"))
		return ;
	std::string uri = args.request->getPath();
	std::string root = args.location.getRoot() + uri;
	AutoIndex autoindex(args.request->getFd(), root, uri); // make sure to pass the right uri
	if (autoindex.fail())
	{
		args.response_code = "500";
		error();
		return ;
	}
	SendResponse(autoindex.getOutput(), -1, args.request->getFd());
}

void Response::statusLine( std::string code )
{
	oss << StatusCodes().getStatusLine(code);
}

int64_t Response::get_file_size( void )
{
	if (ffd == -1)
		return -1;
	struct stat stat_buf;
	if (fstat(ffd, &stat_buf) != 0)
	{
		close(ffd);
		return -1;
	}
	return stat_buf.st_size;
}

Response::Response( config &args ) : args(args)
{
	// args.autoindex = args.location.getAutoindex()? "on" : "off";
	std::cout << "\nresp called->>" << args.request->getFd() << "\n" << std::endl;
	if (args.cgi)
	{
		std::cout << "cgi called->>" << args.request->getFd() << std::endl;
		runCGI();
		return ;
	}
	else if (args.autoindex == "on")
	{
		autoIndex();
		return ;
	}
	switch (args.response_code[0])
	{
		case '2':
			file();
			break;
		case '3':
			redirect();
			break;
		case '4':
		case '5':
			std::cerr << "default break " << args.response_code << std::endl;
			error();
			break;
		default:
			break;
	}
}

void Response::file( void )
{
	ffd = open(args.translated_path.c_str(), O_RDONLY);
	fcntl(ffd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int64_t file_size = get_file_size();
	if (file_size == -1)
	{
		args.response_code = "500";
		error();
		return ;
	}
	statusLine(args.response_code);
	oss << "Content-Length: " << file_size << "\r\n";
	oss << "Content-Type: " << getMediaType(args.requested_path) << "\r\n";
	oss << "Cache-Control: no-store\r\n";
	oss << "\r\n";
	std::cout << "RESPONSE::file->" << ffd << std::endl;
	SendResponse(oss.str(), ffd, args.request->getFd());
}

void Response::redirect( void )
{
	statusLine(args.response_code);
	oss << "Location: " << args.requested_path << "\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), -1, args.request->getFd());
}

void Response::error( void )//5xx 4xx
{
	// int n;
	// std::stringstream num(args.response_code);
	// num >> n;
	args.translated_path = "./html/error.html";// args.Server->getErrorPage(500);
	std::cout << "RESPONSE::error->" << args.translated_path << "$" <<  std::endl;
	ffd = open(args.translated_path.c_str(), O_RDONLY);//open error page
	int64_t file_size = get_file_size();
	if (file_size == -1)
	{
		statusLine("500");
		oss << "Content-Length: " << 0 << "\r\n";
		oss << "Content-Type: " << "text/html" << "\r\n";
		oss << "\r\n";
		SendResponse(oss.str(), -1, args.request->getFd());
		return ;
	}
	statusLine(args.response_code);
	oss << "Content-Length: " << file_size << "\r\n";
	oss << "Content-Type: " << getMediaType(args.translated_path) << "\r\n";
	oss << "Cache-Control: no-store\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), ffd, args.request->getFd());
}

bool Response::onPollout( int sfd )
{
	// std::cout << "onPollout " << sfd << std::endl;
	return SendResponse::send(sfd);
}