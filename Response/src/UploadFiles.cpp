/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadFiles.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:31:28 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/05 14:54:42 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/UploadFiles.hpp"
// https://datatracker.ietf.org/doc/html/rfc7231#section-4.3.3

UploadFiles::UploadFiles(config &args) : args(args) {
	is_upload_request = false;
	if ( ! isMultipartAndValid())
		return;
	std::string body = args.request->getRequestBody();
	requestBody(body);
}

bool UploadFiles::isUploadRequest( void )
{
	return is_upload_request;
}

void UploadFiles::writeToFile( std::string &body )
{
	std::ofstream *file = new std::ofstream();
	size_t pos = args.translated_path.find_last_of("/");
	args.translated_path.erase(pos + 1);
	args.translated_path.append(filename);
	file->open(args.translated_path.c_str(), std::ios::out | std::ios::binary | std::ios::trunc);
	if ( ! file->is_open())
	{
		// args.response_code = "500";
		args.response_code = "500";
		// args.special_headers.clear();
		return ;
	}
	file->write(body.c_str(), body.length());
	file->close();
	if (args.uploaded_file_path.empty())
		args.uploaded_file_path = std::string("/upload-path-not-set/") + filename;
	delete file;
}

bool UploadFiles::bodyContainsUploadedFile( std::string& headers)
{
	std::cout << "headers->" << std::endl;
	size_t pos = headers.find("Content-Disposition:");
	if (pos == std::string::npos)
	{
		std::cout << "invalid request: Content-Disposition not found" << std::endl;
		return false;
	}
	headers.erase(0, pos + 20);
	pos = headers.find("\r\n");//always exists
	std::string line = headers.substr(0, pos);
	// std::cout << "line=" << line << std::endl;
	pos = line.find(";");//: form-data;
	size_t pos2 = line.find_first_not_of(" \t");
	std::string formdata = line.substr(pos2, pos - pos2);
	formdata.erase(formdata.find_last_not_of(" \t;") + 1);
	if (pos == std::string::npos || formdata != "form-data")
	{
		std::cout << "invalid request: form-data not found-->" << formdata << "<" << std::endl;
		return false;
	}
	line.erase(0, line.find_first_not_of(" \t", pos + 1));
	pos = line.find("filename=\"");
	if (pos == std::string::npos)
	{
		std::cout << "invalid request: filename not found" << std::endl;
		return false;
	}
	filename = line.substr(pos + 10);
	line.erase(pos);
	pos = line.find_last_not_of(" \t");
	if (pos != std::string::npos && line[pos] != ';')
	{
		std::cout << "invalid request: xxx filename not found" << std::endl;
		return false;
	}
	pos = filename.find("\"");
	if (pos == std::string::npos)
	{
		std::cout << "invalid request: vvv filename not found" << std::endl;
		return false;
	}
	filename.erase(pos);
	if (filename.empty())
	{
		std::cout << "invalid request: filename is empty" << std::endl;
		args.response_code = "400";
		// args.special_headers.clear();
		return false;
	}
	std::cout << "filename=" << filename << std::endl;
	// std::cout << "filename --> "  << filename << std::endl;
	return true;
}

bool UploadFiles::boundaryBody( std::string &body )
{
	// std::cout << "boundaryBody -->"  << std::endl;
	size_t pos = body.find("\r\n\r\n");
	if (pos == std::string::npos || pos == 0)
	{
		std::cout << "invalid request: end of headers not found" << std::endl;
		args.response_code = "400";
		// args.special_headers.clear();
		return false;
	}
	std::string header = body.substr(0, pos + 4);
	body.erase(0, pos + 4);
	if ( ! bodyContainsUploadedFile(header))
		return false;
	is_upload_request = true;
	// args.response_code = "201";
	// args.multi_status[args.requested_path + filename] =  ;
	// args.special_headers.push_back( std::string("Location:") + filename);
	writeToFile(body);
	return true;
}

void UploadFiles::requestBody( std::string &body )
{
	// std::cout << "requestBody" << std::endl;
	size_t pos = body.find(end_boundary);
	if (pos == std::string::npos)
	{
		std::cout << "invalid request: end boundary not found" << std::endl;
		args.response_code = "400";
		// args.special_headers.clear();
		return; //invalid request
	}
	body.erase(pos);
	pos = body.find(boundary);
	if (pos == std::string::npos)
	{
		std::cout << "invalid request: boundary not found" << std::endl;
		args.response_code = "400";
		// args.special_headers.clear();
		return; //invalid request
	}
	body.erase(0, pos + boundary.length());
	while (pos != std::string::npos)
	{
		pos = body.find(boundary);
		std::string buff = body.substr(0, pos);
		body.erase(0, pos + boundary.length());
		boundaryBody(buff);
		// if (args.response_code[0] == '5' || args.response_code[0] == '4')
		// 	return;
	}
}

bool UploadFiles::isMultipartAndValid( void )
{
	// std::map<std::string, std::string> mp = args.request->getRequestHeaders();
	// std::map<std::string, std::string>::iterator it = mp.find("Content-Type:");
	std::string content = args.request->getContentType();
	// std::string content = " multipart/form-data; boundary=----WebKitFormBoundaryQfKkHui0vxFv0FvE";
	content.erase(content.length() - 2);//\r\n
	size_t pos = content.find_first_not_of(" \t");
	content.erase(0, pos);
	pos = content.find(";");
	std::string multipart = content.substr(0, pos);
	// std::cout << "multipart=" << multipart << std::endl;
	content.erase(0, pos + 1);
	pos = multipart.find_last_not_of(" \t");
	multipart.erase(pos+1);
	if (multipart != "multipart/form-data")
	{
		std::cout << "not my request: suuccess" << multipart << std::endl;
		return false;//not my request: suuccess
	}
	pos = content.find_first_not_of(" \t");
	content.erase(0, pos);
	if (content.compare(0, 9, "boundary=") != 0)
	{
		std::cout << "invalid request: boundary not found" << std::endl;
		args.response_code = "400";
		// args.special_headers.clear();
		return false;//invalid request
	}
	content.erase(0, 9);
	if (content.empty())
	{
		std::cout << "invalid request: boundary is empty" << std::endl;
		args.response_code = "400";
		// args.special_headers.clear();
		return false;//invalid request
	}
	boundary = std::string("--") + content;
	end_boundary =  boundary + std::string("--\r\n");
	boundary.append("\r\n");
	// std::cout << "boundary=" << boundary << std::endl;
	// std::cout << "end_boundary=" << end_boundary << std::endl;
	return true;
}

