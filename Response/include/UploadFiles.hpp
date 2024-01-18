/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadFiles.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:31:46 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/18 01:17:38 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "../../includes/webserv.hpp"
#include "../../includes/Request.hpp"

class FilesUpload
{
    private:

    std::vector<std::string>    _uploaded_files;
    std::string                 _upload_path;
    std::string                 _filename;
    std::string                 _boundary;
    std::string                 _end_boundary;
    std::string                 *_body;
    t_config                    &conf;

    public:

    FilesUpload( t_config &conf );
    bool isUploadRequest( void );
    bool isValidContentType( void );
    void removeFilesOnError( void );
    void writeToFile( std::string &body );
    bool bodyContainsUploadedFile( std::string& headers );
    bool boundaryBody( std::string &body );
    void requestBody( std::string &body );
};