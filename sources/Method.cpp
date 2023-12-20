/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 17:07:51 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/19 19:50:47 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Method.hpp"

Method::Method(t_config config_file)
{
	this->cgi = false;
	this->file_type = "";
	this->location = "";
	this->config_file = config_file;
}

Method::~Method()
{
}

bool	Method::get_method_location(std::string location)
{
	if (this->location == location)
		return (true);
	return (false);
}

std::string	Method::get_method_file_type()
{
	struct stat info;
	stat(this->location.c_str(), &info);
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

	return (this->file_type);
}

bool	Method::has_indexfile()
{
	if (this->config_file.indexfile == "")
		return (false);
	else
		return (true);
}

bool	Method::has_autoindex()
{
	if (this->config_file.autoindex == "on")
	{
		this->autoindex = true;
		return (true);
	}
	else
		return (false);
}

bool			Method::has_cgi()
{
	if (this->location.find(".py") != std::string::npos || this->location.find(".php") != std::string::npos)
	{
		this->cgi = true;
		return (true);
	}
	else
		return (false);
}

std::string		Method::get_status_code()
{
	if (this->file_type == "dir")
	{
		if (this->has_indexfile())
			this->_status_code = "200";
		else if (this->has_autoindex())
			this->_status_code = "200";
		else
			this->_status_code = "403";//forbidden
	}
	else if (this->file_type == "file")
	{
		this->_status_code = "200";
	}
	else
	{
		this->_status_code = "404";//not found
	}
	return (this->_status_code);
}
