/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:52:02 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/29 06:00:36 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
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
		Method(t_config &config_file, int for_delete);
		~Method();

		bool				DeleteFolderContents(const std::string& directoryPath);
		std::string			get_status_code();
		std::string			get_method_location();
		bool				get_method_file_type();
		bool				has_cgi();
		bool				getCGI() const;
		bool				hasIndexFiles();

		bool				has_autoindex();
		bool				hasValidCGI(const std::string& filename);

};



