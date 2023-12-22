/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:52:02 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/22 20:49:28 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Server.hpp"
#include "webserv.hpp"

class Method
{
	private:
		std::string		sys_location;
		std::string		file_type;
		bool			cgi;
		t_config		 &method_config;

	public:
		Method(t_config &config_file);
		~Method();

		std::string			get_status_code();
		std::string			get_method_location();
		bool			get_method_file_type();
		bool			has_cgi();
		bool			getCGI() const;
		bool			hasIndexFiles();

		t_config			getTconfig() const;
		
		bool	has_autoindex();
};

