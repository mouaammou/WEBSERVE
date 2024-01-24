/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewCGI.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 05:06:18 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/24 09:22:23 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <signal.h>
#include <ctime>
#include <sys/time.h>
#include "ParseCGIOutput.hpp"
#include "../../includes/Request.hpp"
#include "Execute.hpp"

class NewCGI : public Execute
{
    public:

    static std::map<int, NewCGI*>   active_procs;
    static std::vector<pid_t>       pids_to_remove;
    int                         socketfd;
    const int                   MAX_SEC_TO_TIMEOUT;
    std::string                 filename;
    std::string                 ifilename;
    t_config                    &conf;
    pid_t                       pid;
    int64_t                     timeout_start;
    bool one_time_kill;

    NewCGI( t_config &conf );

    static void checkExitedProcess( void );
    static void remove( int sfd );
    static void build( config &conf );

    void onProcessExit(int status);
    bool execute( void);
    void child( void );
    int64_t getCurrentTime( void );
    void timeout( void );
    
    pid_t getPid( void );
    void setPid( pid_t pid );
    void closeFiles( void );
};
