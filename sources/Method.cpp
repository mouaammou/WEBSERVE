/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:51 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/26 03:07:50 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Method.hpp"

Method::Method(t_config &config_file): method_config(config_file)
{
	method_config.cgi = false;
	this->sys_location = config_file.translated_path;
	this->method_config.autoindex = "off";
	if (this->get_method_file_type())
	{
		if (this->has_cgi() || this->file_type == "file")
		{
			return ;
		}
		if (this->file_type == "dir")
		{
			if (this->sys_location[this->sys_location.length() - 1] != '/')
			{
				this->method_config.response_code = "301 Moved Permanently";
				this->method_config.translated_path += "/";
				return ;
			}
			else if (this->hasIndexFiles())
			{
				if (this->method_config.translated_path.find(".py") != std::string::npos
					|| this->method_config.translated_path.find(".php") != std::string::npos)
					method_config.cgi = true;
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

bool		Method::get_method_file_type()
{
	struct stat info;
	if (stat(this->sys_location.c_str(), &info) == 0)//need check permissions
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
		std::string tmp = 
			this->method_config.req_location[this->method_config.req_location.length() - 1] == '/' ? 
				this->method_config.req_location.substr(0, this->method_config.req_location.length() - 1) 
				: this->method_config.req_location;
		if (tmp == this->method_config.server_locations[i].getName())
		{
			if (this->method_config.server_locations[i].getAutoindex())
				return (true);
		}
	}
	return (false);
}

bool			Method::has_cgi()
{
	if (this->sys_location.find(".py") != std::string::npos || this->sys_location.find(".php") != std::string::npos)
	{
		method_config.cgi = true;
		return (true);
	}
	return (false);
}

