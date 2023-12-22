/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:51 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/22 20:34:22 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Method.hpp"

Method::Method(t_config config_file)
{
	this->sys_location = config_file.translated_path;
	this->config_file = config_file;
	if (this->get_method_file_type())
	{
		this->config_file.response_code = this->_status_code;
		if (this->has_cgi() || this->file_type == "file")
			return ;
		if (this->file_type == "dir")
		{
			if (this->hasIndexFiles())
			{
				this->_status_code = "200 Ok";
				return ;
			}
			else if (this->has_autoindex())
			{
				this->_status_code = "200 Ok";
				this->config_file.autoindex = "on";
				return ;
			}
			else
			{
				this->_status_code = "403 Forbidden";
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
	for (size_t i = 0; i < this->config_file.server_locations.size(); i++)
	{
		if (this->config_file.req_location == this->config_file.server_locations[i].getName())
		{
			if (this->config_file.server_locations[i].getIndex() != "")
			{
				this->config_file.translated_path += this->config_file.server_locations[i].getIndex();
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

t_config			Method::getTconfig() const
{
	return (this->config_file);
}

bool		Method::get_method_file_type()
{
	struct stat info;
	if (stat(this->sys_location.c_str(), &info) == 0)
	{
		if (S_ISDIR(info.st_mode))
		{
			this->file_type = "dir";
		}
		else if (S_ISREG(info.st_mode))
		{
			this->file_type = "file";
		}
		else
		{
			this->file_type = "none";
		}
	}
	else
	{
		this->_status_code = "404 Not Found";
		return (false);
	}
	this->_status_code = "200 Ok";
	return (true);
}

bool	Method::has_autoindex()
{
	for (size_t i = 0; i < this->config_file.server_locations.size(); i++)
	{
		if (this->config_file.req_location == this->config_file.server_locations[i].getName())
		{
			if (this->config_file.server_locations[i].getAutoindex())
				return (true);
		}
	}
	return (false);
}

bool			Method::has_cgi()
{
	if (this->sys_location.find(".py") != std::string::npos || this->sys_location.find(".php") != std::string::npos)
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

std::string		Method::get_status_code()
{
	return (this->_status_code);
}
