/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/11 16:52:02 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/13 04:31:32 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Server.hpp"
#include "webserv.hpp"
#include "../Response/include/UploadFiles.hpp"

class Method
{
	private:
		std::string		file_type;
		t_config		 &method_config;
		std::string		p_path;


	public:
		Method(t_config &config_file);

		~Method();


		void 				getMethod();
		void 				deleteMethod();
		void 				postMethod();
		bool				deleteFolderContents(const std::string& directoryPath);
		std::string			get_method_location();
		bool				get_method_file_type();
		bool				has_cgi();
		bool				getCGI() const;
		bool				hasIndexFiles();

		bool				has_autoindex();
		bool				hasValidCGI(const std::string& filename);
};



