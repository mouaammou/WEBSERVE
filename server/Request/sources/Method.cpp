/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:51 by mouaammo          #+#    #+#             */
/*   Updated: 2024/02/18 03:23:11 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Method.hpp"
#include <cstdio>
#include <sys/unistd.h>

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

bool Method::deleteFolderContents(const std::string& directoryPath)
{
	DIR* dir = opendir(directoryPath.c_str());
	if (dir == NULL)
	{
		this->method_config.response_code = "500 Internal Server Error";
		return false;
	}
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
					if (!deleteFolderContents(fullPath))
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
		std::cerr << "not removed" << std::endl;
	return true;
}

Method::Method(t_config &config_file): method_config(config_file)
{
    this->method_config.autoindex = "off";
    this->method_config.cgi = false;
    this->file_type = "none";
    this->has_cgi();
    if (this->get_method_file_type())
        this->is_status_ok = true;
    else
        this->is_status_ok = false;
    if (this->file_type == "dir")
    {
        if (method_config.requested_path[method_config.requested_path.length() - 1] != '/')
        {
            this->method_config.response_code = "301 Moved Permanently";
            this->method_config.requested_path += "/";
            return ;
        }
    }
    if (this->has_autoindex() && this->file_type == "dir")
    {
        this->method_config.autoindex = "on";
    }
}

void Method::postMethod()
{
	FilesUpload(this->method_config).isUploadRequest();//upload file is supported.
}


void Method::deleteMethod()
{
    std::string parent_dir = this->method_config.translated_path.rfind("/") == 0 ? "/" : this->method_config.translated_path.substr(0, this->method_config.translated_path.rfind("/"));
	if (is_status_ok)
	{
		if (this->file_type == "file")
		{
            if (access(parent_dir.c_str(), W_OK) == -1 
                || access(parent_dir.c_str(), X_OK) == -1)
            {
                this->method_config.response_code = "403 Forbidden";
                return ;
            }
			if ( ! this->method_config.cgi)
			{
				if (remove(this->method_config.translated_path.c_str()))
                    this->method_config.response_code = "500 Internal Server Error";
                else
                    this->method_config.response_code = "204 No Content";
			}
		}
		else if (this->file_type == "dir")
		{
            //check if the folder has all the permissions
            if (access(this->method_config.translated_path.c_str(), W_OK) == -1 
                || access(this->method_config.translated_path.c_str(), R_OK) == -1 
                || access(this->method_config.translated_path.c_str(), X_OK) == -1)
            {
                this->method_config.response_code = "403 Forbidden";
                return ;
            }
            if (deleteFolderContents(this->method_config.translated_path))//
                this->method_config.response_code = "204 No Content";
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
	if (stat(this->method_config.translated_path.c_str(), &info) == 0)
	{
        //check if the file is readable and writable
        if (access(this->method_config.translated_path.c_str(), R_OK) == -1 && this->method_config.request->getMethod() != "DELETE")
        {
            this->method_config.response_code = "403  ssForbidden";
            return (false);
        }
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
	return (true);
}

bool	Method::has_autoindex()
{
	for (size_t i = 0; i < this->method_config.server_locations.size(); i++)
	{
		if (this->method_config.requested_path == this->method_config.server_locations[i].getName())
		{
			if (this->method_config.server_locations[i].getAutoindex() == 1)
				return (true);
			else if (this->method_config.server_locations[i].getAutoindex() == 0
			|| this->method_config.server_locations[i].getAutoindex() == -1)
			{
				return (false);
			}
		}
	}
	for (size_t i = 0; i < this->method_config.server_locations.size(); i++)
	{
		if (this->method_config.server_locations[i].getName() == "/")
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
		this->method_config.cgi = true;
		return (true);
	}
	this->method_config.cgi = false;
	return (false);
}

bool			Method::getCGI() const
{
	return (this->method_config.cgi);
}

