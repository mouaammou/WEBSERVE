/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/21 01:08:15 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/24 11:42:28 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../../socket/include/webserv.hpp"
#include "SendResponse.hpp"
#include "AutoIndex.hpp"
#include "../../cgi/include/NewCGI.hpp"
#include "Codes.hpp"
#include "MediaTypes.hpp"
#include "CacheControl.hpp"

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
	void allow( void );

	public:

	static std::string getDate( void );

	Response( config &args );
	void runCGI( void );
	void autoIndex( void );
	void file( void );
	void redirect( void );
	void error( void );
};