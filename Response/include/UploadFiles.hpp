/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadFiles.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:31:46 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/13 00:55:01 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "../../includes/webserv.hpp"
#include "../../includes/Request.hpp"

// class UploadFiles
// {
// 	private:

// 	config		&args;
// 	std::string	boundary;
// 	std::string	end_boundary;
// 	std::string filename;
// 	bool		is_upload_request;

// 	public:

// 	UploadFiles( config &args );

// 	bool isUploadRequest( void );

// 	private:

// 	bool bodyContainsUploadedFile( std::string& headers);
// 	bool boundaryBody( std::string &body );
// 	void requestBody( std::string &body );
// 	void writeToFile( std::string &body );
// 	bool isMultipartAndValid( void );
// };

class FilesUpload
{
    private:

    // 422 Unprocessable Content
    std::vector<std::string>    _uploaded_files;
    std::string                 _upload_path;//
    std::string                 _filename;//
    std::string                 _boundary;//
    std::string                 _end_boundary;////
    std::string                 *_body;////
    t_config                    &conf;

    public:

    FilesUpload( t_config &conf );// 
    bool isUploadRequest( void );
    bool isValidContentType( void );//
    void removeFilesOnError( void );
    void writeToFile( std::string &body );
    bool bodyContainsUploadedFile( std::string& headers );
    bool boundaryBody( std::string &body );
    void requestBody( std::string &body );//
};