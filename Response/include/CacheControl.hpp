/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CacheControl.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 19:57:32 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/13 20:18:36 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <ctime>
#include <cstring>
#include <sys/stat.h>
#include "../../includes/webserv.hpp"
#include "../../includes/Request.hpp"

class CacheControl
{
    int         _ffd;
    t_config    &_conf;

    std::time_t getLastModified( int fd );
    std::string getCacheHeader( void );
    time_t convertToUnixTimestamp( void );
    void trimSpaces( std::string& str );

    public:

    CacheControl( t_config &conf, int ffd );
    std::string getfileLastModificationDate( int fd ); // use it on 200;
    bool isModifiedSince( void ); // use it to check if 304 or 200;
};
