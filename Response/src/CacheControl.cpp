/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CacheControl.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/13 19:57:32 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/22 14:04:44 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/CacheControl.hpp"

std::time_t CacheControl::getLastModified( int fd )
{
    struct stat fileStat;
    fstat(fd, &fileStat);
    std::time_t lastModified = fileStat.st_mtime;
    return lastModified;
}

std::string CacheControl::getCacheHeader( void )
{
    std::map <std::string, std::string> &headers = _conf.request->getRequestHeaders();
    if (headers.find("If-Modified-Since:") == headers.end())
        return "";
    std::string str = headers["If-Modified-Since:"];   
    trimSpaces(str);
    return str;
}

time_t CacheControl::convertToUnixTimestamp( void )
{
    struct tm tmStruct;
    std::memset(&tmStruct, 0, sizeof(struct tm));
    
    // "Wed, 15 Dec 2021 12:00:00 GMT"
    const char* format = "%a, %d %b %Y %H:%M:%S %Z";
    if ( ! strptime(getCacheHeader().c_str(), format, &tmStruct))
        return 0;
    time_t timestamp = mktime(&tmStruct);
    return timestamp;
}


CacheControl::CacheControl( t_config &conf, int ffd ) : _conf(conf)
{
    _ffd = ffd;
}

std::string CacheControl::getfileLastModificationDate( int fd ) // use it on 200
{
    std::time_t lastModified = getLastModified(fd);
    std::tm* utcTime = std::gmtime(&lastModified);
    char buffer[120]; 
    std::memset(buffer, 0, sizeof(buffer));
    std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S GMT", utcTime);
    // Last-Modified: Tue, 15 Nov 1994 12:45:26 GMT
    return buffer;
}

bool CacheControl::isModifiedSince( void ) // use it to check if 304 or 200
{
    std::string ifModifiedSince = getCacheHeader();
    if (ifModifiedSince.empty())
        return true;
    // file modified since last request
    std::time_t lastModified = getLastModified(_ffd);
    if (lastModified  > convertToUnixTimestamp())
        return true;
    _conf.response_code = "304";// Not Modified
    return false;
}

void CacheControl::trimSpaces( std::string& str )
{
    std::string::size_type start = str.find_first_not_of(" \t\r\n");
    if (start != std::string::npos) {
        str = str.substr(start);
    }
    else
    {
        str.clear();
        return;
    }
    std::string::size_type end = str.find_last_not_of(" \t\r\n");
    if (end != std::string::npos)
        str = str.substr(0, end + 1);
}
