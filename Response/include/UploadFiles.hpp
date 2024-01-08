/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadFiles.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:31:46 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/05 12:33:13 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "../../includes/webserv.hpp"
#include "../../includes/Request.hpp"

class UploadFiles
{
	private:

	config		&args;
	std::string	boundary;
	std::string	end_boundary;
	std::string filename;
	bool		is_upload_request;

	public:

	UploadFiles( config &args );

	bool isUploadRequest( void );

	private:

	bool bodyContainsUploadedFile( std::string& headers);
	bool boundaryBody( std::string &body );
	void requestBody( std::string &body );
	void writeToFile( std::string &body );
	bool isMultipartAndValid( void );
};
