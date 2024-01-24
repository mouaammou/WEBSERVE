/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/18 12:22:50 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/24 18:27:16 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Response.hpp"

void Response::runCGI(void)
{
	NewCGI::build(args);
}

void Response::autoIndex(void)
{
	std::string uri = args.request->getPath();
	std::string root = args.location.getRoot() + uri;
	AutoIndex autoindex(args.request->getFd(), root, uri);
	if (autoindex.fail())
	{
		args.response_code = "500";
		error();
		return;
	}
	SendResponse(autoindex.getOutput(), -1, args.request->getFd());
}

void Response::statusLine(std::string code)
{
	std::cout << "        Status:   [" << StatusCodes().getStatusLine(code);
	oss << StatusCodes().getStatusLine(code);
}

int64_t Response::get_file_size(void)
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

Response::Response(config &args) : args(args)
{
	args.socket_fd = args.request->getFd();
	if (args.cgi)
	{
		runCGI();
		return;
	}
	else if (args.autoindex == "on")
	{
		autoIndex();
		return;
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
		error();
		break;
	default:
		break;
	}
}

void Response::file(void)
{
	if (args.response_code.compare(0, 3, "204") == 0)
	{
		statusLine(args.response_code);
		oss << "Content-Length: " << 0 << "\r\n";
		oss << "Content-Type: "
			<< "text/plain"
			<< "\r\n";
		oss << "Date: " << getDate() << "\r\n";
		oss << "Server: "
			<< "Webserv/1.0"
			<< "\r\n";
		oss << "\r\n";
		SendResponse(oss.str(), -1, args.request->getFd());
		return;
	}
	if (args.response_code.compare(0, 3, "201") == 0)
	{
		statusLine(args.response_code);
		oss << "Location: " << args.translated_path << "\r\n";
		oss << "Content-Length: " << args.location.getUploadPath().length() + 57 << "\r\n";
		oss << "Content-Type: "
			<< "text/plain"
			<< "\r\n";
		oss << "\r\n";
		oss << "Resource(s) created successfully. you can access it at \r\n";
		oss << args.location.getUploadPath() << "\r\n";
		SendResponse(oss.str(), -1, args.request->getFd());
		return;
	}
	ffd = open(args.translated_path.c_str(), O_RDONLY);
	fcntl(ffd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
	int64_t file_size = get_file_size();
	if (file_size == -1)
	{
		args.response_code = "500";
		error();
		return;
	}
	CacheControl cache(args, ffd); // cache
	if (!cache.isModifiedSince())
	{
		args.response_code = "304";
		statusLine(args.response_code);
		oss << "Cache-Control: public, max-age=1, no-cache, must-revalidate\r\n";
		oss << "Date: " << getDate() << "\r\n";
		oss << "Last-Modified: " << CacheControl(args, ffd).getfileLastModificationDate(ffd) << "\r\n";
		oss << "Accept-Ranges: none\r\n";
		oss << "Server: "
			<< "Webserv/1.0"
			<< "\r\n";
		oss << "\r\n";
		close(ffd);
		SendResponse(oss.str(), -1, args.request->getFd());
		return;
	} // cache
	statusLine(args.response_code);
	oss << "Content-Length: " << file_size << "\r\n";
	oss << "Content-Type: " << getMediaType(args.translated_path) << "\r\n";
	oss << "Cache-Control: public, max-age=1, no-cache, must-revalidate\r\n";
	oss << "Date: " << getDate() << "\r\n";
	oss << "Last-Modified: " << CacheControl(args, ffd).getfileLastModificationDate(ffd) << "\r\n";
	oss << "Accept-Ranges: none\r\n";
	oss << "Server: "
		<< "Webserv/1.0"
		<< "\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), ffd, args.request->getFd());
}

void Response::redirect(void)
{
	statusLine(args.response_code);
	oss << "Location: " << args.requested_path << "\r\n";
	if (args.response_code.compare(0, 3, "301") == 0)
		oss << "Retry-After: 120\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), -1, args.request->getFd());
}

void Response::allow(void)
{
	if (args.response_code.compare(0, 3, "405") != 0)
		return;
	std::string allowed = "Allow: ";
	std::vector<std::string> methods = args.location.getMethods();
	for (size_t i = 0; i < methods.size(); i++)
	{
		allowed.append(methods[i]);
		if (i != methods.size() - 1)
			allowed.append(", ");
	}
	oss << allowed << "\r\n";
}

void Response::error(void) // 5xx 4xx
{
	int n;
	std::stringstream num(args.response_code);
	num >> n;
	args.translated_path = args.Server->getErrorPage(n);
	if (!args.translated_path.empty())
		ffd = open(args.translated_path.c_str(), O_RDONLY);
	int64_t file_size = get_file_size();
	if (args.translated_path.empty() || file_size == -1)
	{
		std::string error = StatusCodes().getStatusLine(args.response_code);
		error = std::string("<!DOCTYPE html>\n<html>\n<body>\n\t<h1>\n\t\t") + error.erase(0, 9) + std::string("\t</h1>\n</body>\n</html>\n");
		statusLine(args.response_code);
		oss << "Content-Length: " << error.length() << "\r\n";
		oss << "Content-Type: "
			<< "text/html"
			<< "\r\n";
		oss << "Cache-Control: no-store\r\n";
		oss << "Server: "
			<< "Webserv/1.0"
			<< "\r\n";
		allow();
		oss << "Date: " << getDate() << "\r\n";
		oss << "\r\n";
		oss << error;
		SendResponse(oss.str(), -1, args.request->getFd());
		return;
	}
	statusLine(args.response_code);
	oss << "Content-Length: " << file_size << "\r\n";
	oss << "Content-Type: " << getMediaType(args.translated_path) << "\r\n";
	oss << "Cache-Control: no-store\r\n";
	oss << "Server: "
		<< "Webserv/1.0"
		<< "\r\n";
	allow();
	oss << "Date: " << getDate() << "\r\n";
	oss << "\r\n";
	SendResponse(oss.str(), ffd, args.request->getFd());
}

bool Response::onPollout(int sfd)
{
	return SendResponse::send(sfd);
}

std::string Response::getDate(void)
{
	std::time_t currentTime = std::time(NULL);
	std::tm *utcTime = std::gmtime(&currentTime);
	char buffer[120];
	std::memset(buffer, 0, sizeof(buffer));
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", utcTime);
	// Date: Tue, 15 Nov 1994 08:12:31 GMT
	return buffer;
}