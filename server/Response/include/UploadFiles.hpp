/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UploadFiles.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/05 12:31:46 by samjaabo          #+#    #+#             */
/*   Updated: 2024/02/14 15:27:03 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "../../socket/include/webserv.hpp"
#include "../../Request/includes/Request.hpp"

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