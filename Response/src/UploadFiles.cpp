/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadFiles.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:31:28 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/22 18:20:42 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/UploadFiles.hpp"

FilesUpload::FilesUpload( t_config &conf ) : conf(conf)
{
	if ( ! isUploadRequest())
		return ;
	this->_filename = "";
	this->_boundary = "";
	this->_end_boundary = "";
	this->_body = &conf.request->getRequestBody();
	this->_upload_path = conf.location.getRoot() + conf.location.getUploadPath();
	if ( ! this->isValidContentType())
	{
		conf.response_code = "400";
		return ;
	}
	std::string body = conf.request->getRequestBody();
	requestBody(*_body);
}

bool FilesUpload::isUploadRequest( void )
{
	/*** use outside to check if request is for files upload ***/
	if (conf.cgi)
		return (false);
	if (conf.location.getUploadPath().empty())
		return (false);
	return (true);
}

bool FilesUpload::isValidContentType( void )
{
	/**** false means invalid request (code 400) ****/
	std::string content_type = conf.request->getContentType();
	size_t pos = content_type.find_first_not_of(" \t");
	if (pos != std::string::npos)
		content_type.erase(0, pos);
	pos = content_type.find(";");
	if (pos == std::string::npos)
		return (false);
	std::string multipart = content_type.substr(0, pos);
	std::string boundary = content_type.substr(pos + 1);
	pos = multipart.find_last_not_of(" \t");
	if (pos != std::string::npos)
		multipart.erase(pos + 1);
	if (multipart != "multipart/form-data")
		return (false);
	pos = boundary.find_first_not_of(" \t");
	if (pos != std::string::npos)
		boundary.erase(0, pos);
	pos = boundary.find_last_not_of(" \t\r\n");
	if (pos != std::string::npos)
		boundary.erase(pos + 1);
	if (boundary.compare(0, 9, "boundary=") != 0)
		return (false);
	boundary.erase(0, 9);
	this->_boundary = std::string("--") + boundary;
	if (this->_boundary.empty())
		return (false);
	this->_end_boundary = this->_boundary + std::string("--\r\n");
	this->_boundary += "\r\n";
	return (true);
}

void FilesUpload::removeFilesOnError( void )
{
	for (size_t i = 0; i < _uploaded_files.size(); i++)
		std::remove((_upload_path + _uploaded_files[i]).c_str());
}

void FilesUpload::writeToFile( std::string &body )
{
	std::ofstream *file = new std::ofstream();
	file->open(_upload_path + _filename, std::ios::out | std::ios::binary | std::ios::trunc);
	if ( ! file->is_open())
	{
		conf.response_code = "500";
		delete file;
		return ;
	}

	file->write(body.c_str(), body.length());
	if (file->fail())
	{
		conf.response_code = "500";
		delete file;
		return ;
	}
	file->close();
	_uploaded_files.push_back(_filename);
	if (conf.uploaded_file_path.empty())
		conf.translated_path = _upload_path + _filename;
	delete file;
}

bool FilesUpload::bodyContainsUploadedFile( std::string& headers )
{
	size_t pos = headers.find("Content-Disposition:");
	if (pos == std::string::npos)
		return false;
	headers.erase(0, pos + 20);
	pos = headers.find("\r\n");
	std::string line = headers.substr(0, pos);
	pos = line.find(";");//: form-data;
	size_t pos2 = line.find_first_not_of(" \t");
	std::string formdata = line.substr(pos2, pos - pos2);
	formdata.erase(formdata.find_last_not_of(" \t;") + 1);
	if (pos == std::string::npos || formdata != "form-data")
		return false;
	line.erase(0, line.find_first_not_of(" \t", pos + 1));
	pos = line.find("filename=\"");
	if (pos == std::string::npos)
		return false;
	_filename = line.substr(pos + 10);
	line.erase(pos);
	pos = line.find_last_not_of(" \t");
	if (pos != std::string::npos && line[pos] != ';')
		return false;
	pos = _filename.find("\"");
	if (pos == std::string::npos)
		return false;
	_filename.erase(pos);
	if (_filename.empty())
	{
		conf.response_code = "400";
		return false;
	}
	return true;
}

bool FilesUpload::boundaryBody( std::string &body )
{
	size_t pos = body.find("\r\n\r\n");
	if (pos == std::string::npos || pos == 0)
	{
		conf.response_code = "400";
		return false;
	}
	std::string header = body.substr(0, pos + 4);
	body.erase(0, pos + 4);
	if ( ! bodyContainsUploadedFile(header))
		return false;
	writeToFile(body);
	return true;
}

void FilesUpload::requestBody( std::string &body )
{
	size_t pos = body.find(_end_boundary);
	if (pos == std::string::npos)
	{
		conf.response_code = "400";
		return;
	}
	body.erase(pos);
	pos = body.find(_boundary);
	if (pos == std::string::npos)
	{
		conf.response_code = "400";
		return ; 
	}
	body.erase(0, pos + _boundary.length());
	while (pos != std::string::npos)
	{
		pos = body.find(_boundary);
		std::string buff = body.substr(0, pos);
		body.erase(0, pos + _boundary.length());
		boundaryBody(buff);
		if (conf.response_code[0] == '5' || conf.response_code[0] == '4')
		{
			removeFilesOnError();
			return ;
		}
	}
	if (_filename.empty())
	{
		conf.response_code = "422";
		return ;
	}
	conf.response_code = "201";
}
