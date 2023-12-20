/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:52:02 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/19 19:50:31 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Server.hpp"

class Method
{
	private:
		std::string		_status_code;
		std::string		location;
		std::string		file_type;
		bool			autoindex;
		bool			cgi;
		t_config		 config_file;

	public:
		Method(t_config config_file);
		~Method();

		bool			get_method_location(std::string location);
		std::string		get_method_file_type();
		std::string		get_status_code();
		bool			has_cgi();
		
		bool	has_indexfile();
		bool	has_autoindex();
};

