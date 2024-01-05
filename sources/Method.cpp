/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:51 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/04 05:57:17 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Method.hpp"


bool Method::hasValidCGI(const std::string& filename)
{
	std::string pyExtension = ".py";
	std::string phpExtension = ".php";

	if (filename.length() >= pyExtension.length() &&
		(filename.compare(filename.length() - pyExtension.length(), pyExtension.length(), pyExtension) == 0))
		return true;
	if (filename.length() >= phpExtension.length() &&
		(filename.compare(filename.length() - phpExtension.length(), phpExtension.length(), phpExtension) == 0))
		return true;
	return false;
}



bool Method::DeleteFolderContents(const std::string& directoryPath)
{
	DIR* dir = opendir(directoryPath.c_str());
	if (dir == NULL)
		return false;
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string fileName = entry->d_name;
		if (fileName != "." && fileName != "..")
		{
			std::string fullPath = directoryPath + "/" + fileName;
			struct stat fileStat;
			if (stat(fullPath.c_str(), &fileStat) == 0)
			{
				if (S_ISDIR(fileStat.st_mode))
				{
					if (!DeleteFolderContents(fullPath))
					{
						closedir(dir);
						return false;
					}
				}
				else
				{
					if (remove(fullPath.c_str()) != 0)
					{
						closedir(dir);
						return false;
					}
				}
			}
			else
			{
				closedir(dir);
				return false;
			}
		}
	}
	closedir(dir);
	if (remove(directoryPath.c_str()) != 0)
		std::cerr << "not removed" << std::endl;  /// in case of error what should we do			??????????????????????????
	return true;
}

bool	Method::uploadFiles(void)
{
	std::map<std::string, std::string> mp = method_config.request->getRequestHeaders();
	std::map<std::string, std::string>::iterator it = mp.find("Content-Type:");
	size_t pos = it->second.find("boundary=");
	if (it == mp.end() || it->second.find("multipart/form-data") == std::string::npos
		|| pos == std::string::npos)
		return false;
	std::string boundary = std::string("--") + it->second.substr(pos + 9) + "\r\n";
	std::string end = boundary + "--\r\n";
	std::string body = method_config.request->getRequestBody();
	pos = body.find(end);
	if (pos == std::string::npos)
		return false;
	body.erase(pos);
	pos = body.find(boundary);
	std::cout << "*********************************"<< std::endl;
	while (pos != std::string::npos)
	{
		body.erase(0, pos + boundary.length() - 2);//keep '\r\n'
		pos = body.find("\r\n\r\n", pos);
		if (pos == std::string::npos)
			return false;
		std::string header(body.substr(0, pos));
		pos = header.find("\r\nContent-Disposition:");
		if (pos == std::string::npos)
			return false;
		std::string cd(header.substr(pos + 2, header.find("\r\n", pos + 2) - pos - 2));
		std::cout << "Content-Disposition:" << cd << std::endl;
		pos = body.find(boundary);
	}
	return true;
}

Method::Method(t_config &config_file, std::string post): method_config(config_file)
{
	(void)post;
	this->method_config.autoindex = "off";
	this->method_config.cgi = false;
	if (uploadFiles())//upload file is supported.
	{
		this->method_config.response_code = "201 Created";
	}
	else
	{
		if (this->get_method_file_type())
		{
			if (this->file_type == "file")
			{
				if (!this->has_cgi())
					this->method_config.response_code = "403 Forbidden";
				return ;
			}
			else if (this->file_type == "dir")
			{
				if (this->method_config.translated_path[this->method_config.translated_path.length() - 1] != '/')
				{
					this->method_config.response_code = "301 Moved Permanent";
					return ;
				}
				else if (this->method_config.translated_path[this->method_config.translated_path.length() - 1] == '/')
				{
					if (this->hasIndexFiles())
					{
						if (!this->has_cgi())
							this->method_config.response_code = "403 Forbidden";
						return ;
					}
					else
					{
						this->method_config.response_code = "403 Forbidden";
						return;
					}
				}
			}
		}
	}
}


Method::Method(t_config &config_file, int for_delete): method_config(config_file)
{
	(void)for_delete;
	this->method_config.autoindex = "off";
	this->method_config.cgi = false;
	if (this->get_method_file_type())
	{
		if (this->file_type == "file")
		{
			if (!this->has_cgi())
			{
				if (remove(this->method_config.translated_path.c_str()))
					std::cerr << "remove fails" << std::endl;
				this->method_config.response_code = "204 No Content";
			}
			return ;
		}
		else if (this->file_type == "dir")
		{
			if (this->method_config.translated_path[this->method_config.translated_path.length() - 1] != '/')
			{
				this->method_config.response_code = "409 Conflict";
				return ;
			}
			else if (this->method_config.translated_path[this->method_config.translated_path.length() - 1] == '/')
			{
				if (this->has_cgi())
				{
					if (!this->hasIndexFiles())
						this->method_config.response_code = "403 Forbidden";
					return ;
				}
				else
				{
					if (DeleteFolderContents(this->method_config.translated_path))
						this->method_config.response_code = "204 No Content";
					else
					{
						if (access(this->method_config.translated_path.c_str(), W_OK | R_OK) == 0)
							this->method_config.response_code = "500 Internal Server Error";
						this->method_config.response_code = "403 Forbidden";
					}
				}
			}
		}
	}
}


Method::Method(t_config &config_file): method_config(config_file)
{
	this->method_config.autoindex = "off";
	this->method_config.cgi = false;
	if (this->get_method_file_type())
	{
		if (this->file_type == "file")
		{
			this->has_cgi();
			return ;
		}
		else if (this->file_type == "dir")
		{
			if (method_config.requested_path[method_config.requested_path.length() - 1] != '/')
			{
				this->method_config.response_code = "301 Moved Permanently";
				this->method_config.requested_path += "/";
				return ;
			}
			if (this->hasIndexFiles())
			{
				if (hasValidCGI(this->method_config.translated_path))
				{
					this->method_config.cgi = true;
				}
				return ;
			}
			else if (this->has_autoindex())
			{
				this->method_config.autoindex = "on";
				return ;
			}
			else
			{
				this->method_config.response_code = "403 Forbidden";
				return ;
			}
		}
	}
}

Method::~Method()
{
}

bool			Method::hasIndexFiles()
{
	for (size_t i = 0; i < this->method_config.server_locations.size(); i++)
	{
		if (method_config.requested_path == this->method_config.server_locations[i].getName())
		{
			if (this->method_config.server_locations[i].getIndex() != "")
			{
				this->method_config.translated_path += this->method_config.server_locations[i].getIndex();
				return (true);
			}
		}
	}
	return (false);
}

std::string			Method::get_method_location()
{
	return (this->method_config.translated_path);
}

bool		Method::get_method_file_type()
{
	struct stat info;
	if (stat(this->method_config.translated_path.c_str(), &info) == 0)//check permission
	{
		if (S_ISDIR(info.st_mode))
			this->file_type = "dir";
		else if (S_ISREG(info.st_mode))
			this->file_type = "file";
	}
	else
	{
		this->method_config.response_code = "404 Not Found";
		return (false);
	}
	this->method_config.response_code = "200 Ok";
	return (true);
}

bool	Method::has_autoindex()
{
	for (size_t i = 0; i < this->method_config.server_locations.size(); i++)
	{
		if (this->method_config.requested_path == this->method_config.server_locations[i].getName())
		{
			if (this->method_config.server_locations[i].getAutoindex())
				return (true);
		}
	}
	return (false);
}

bool			Method::has_cgi()
{
	if (hasValidCGI(method_config.translated_path))
	{
		// std::cout << "**** CGI FOUND" << std::endl;
		this->method_config.cgi = true;
		return (true);
	}
	return (false);
}

bool			Method::getCGI() const
{
	return (this->method_config.cgi);
}

