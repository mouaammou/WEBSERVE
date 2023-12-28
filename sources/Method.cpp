/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:51 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/28 05:56:30 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Method.hpp"
#include <string>
#include <string>

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


Method::Method(t_config &config_file, int for_delete): method_config(config_file)
{
	(void)for_delete;
	this->sys_location = config_file.translated_path;
	this->method_config.autoindex = "off";
	this->cgi = false;
	if (this->get_method_file_type())
	{
		if (this->file_type == "file")
		{
			if (!this->has_cgi())
			{
				if (remove(this->sys_location.c_str()))
					perror("Error deleting file");
				this->method_config.response_code = "204 No Content";
			}
			return ;
		}
		else if (this->file_type == "dir")
		{
			if (this->sys_location[this->sys_location.length() - 1] != '/')
			{
				this->method_config.response_code = "409 Conflict";
				return ;
			}
			else if (this->sys_location[this->sys_location.length() - 1] == '/')
			{
				if (this->has_cgi())
				{
					if (!this->hasIndexFiles())
						this->method_config.response_code = "403 Forbidden";
					return ;
				}
				else
				{
					//delete all folder content

				}
			}
		}
	}
}

Method::Method(t_config &config_file): method_config(config_file)
{
	this->sys_location = config_file.translated_path;
	this->method_config.autoindex = "off";
	this->cgi = false;
	if (this->get_method_file_type())
	{
		if (this->file_type == "file")
		{
			this->has_cgi();
			return ;
		}
		else if (this->file_type == "dir")
		{
			if (this->sys_location[this->sys_location.length() - 1] != '/')
			{
				this->method_config.response_code = "301 Moved Permanently";
				this->method_config.translated_path += "/";
				return ;
			}
			if (this->hasIndexFiles())
			{
				if (hasValidCGI(this->method_config.translated_path))
				{
					this->cgi = true;
					return ;
				}
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
		if (this->method_config.req_location == this->method_config.server_locations[i].getName())
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
	return (this->sys_location);
}

// t_config			Method::getTconfig() const
// {
// 	return (this->method_config);
// }

bool		Method::get_method_file_type()
{
	struct stat info;
	if (stat(this->sys_location.c_str(), &info) == 0)//check permission
	{
		if (S_ISDIR(info.st_mode))
			this->file_type = "dir";
		else if (S_ISREG(info.st_mode))
			this->file_type = "file";
		else
			this->file_type = "none";
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
		if (this->method_config.req_location == this->method_config.server_locations[i].getName())
		{
			if (this->method_config.server_locations[i].getAutoindex())
				return (true);
		}
	}
	return (false);
}

bool			Method::has_cgi()
{
	if (hasValidCGI(sys_location))
	{
		this->cgi = true;
		return (true);
	}
	return (false);
}

bool			Method::getCGI() const
{
	return (this->cgi);
}

