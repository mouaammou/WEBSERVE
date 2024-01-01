/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:52:02 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/01 22:18:17 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "webserv.hpp"

class Method
{
	private:
		std::string		file_type;
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



