/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   AutoIndex.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 19:15:36 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/09 11:33:08 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <map>
#include "Codes.hpp"
#include "SendResponse.hpp"

class AutoIndex
{
	private:

	std::ostringstream		fin;
	std::string				dir_path;//physical path
	std::string				uri;//uri path
	bool					error;

	void generate(void);//
	bool isValid( std::string path );
	void files( std::ostringstream &oss );
	std::string formatSize(int64_t bytes);
	std::string dirUriFilter( std::string const filePath );
	std::string nameFilter( std::string const filePath, std::string const name );
	std::string fileLastModificationDate( std::string const filePath );
	int64_t fileSize( std::string const filePath );

	public:

	AutoIndex( int sfd, std::string const &path, std::string const &uri );//
	bool fail( void ) const;
	std::string const getOutput( void ) const;
};
