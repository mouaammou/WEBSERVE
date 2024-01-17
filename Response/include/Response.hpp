/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 01:08:15 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/18 00:35:29 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../includes/webserv.hpp"
#include "SendResponse.hpp"
#include "AutoIndex.hpp"
#include "NewCGI.hpp"
#include "Codes.hpp"
#include "MediaTypes.hpp"
#include "CacheControl.hpp"

// #include "Config.hpp"

class Response
{
	public:

	static bool onPollout( int sfd );

	private:

	std::ostringstream	oss;
	int					ffd;
	config				&args;

	void statusLine( std::string code );
	int64_t get_file_size( void );
	std::string getDate( void );
	void allow( void );

	public:

	Response( config &args );
	void runCGI( void );
	void autoIndex( void );
	void file( void );
	void redirect( void );
	void error( void );
};