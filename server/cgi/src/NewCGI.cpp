/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewCGI.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:01:07 by samjaabo          #+#    #+#             */
/*   Updated: 2024/02/18 20:35:49 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/NewCGI.hpp"
#include <sys/fcntl.h>


std::map<int, NewCGI*>   NewCGI::active_procs;
std::vector<pid_t>          NewCGI::pids_to_remove;

void NewCGI::remove( int sfd )
{
    std::map<int, NewCGI*>::iterator it = active_procs.find(sfd);
    if (it == active_procs.end())
        return ;
    NewCGI *cgi = it->second;
    if (cgi->pid != -1)
        kill(cgi->pid, SIGKILL);
    pids_to_remove.push_back(cgi->getPid());
    active_procs[cgi->socketfd ] = NULL;
    delete cgi;
    active_procs.erase(sfd);
    SendResponse::remove(sfd);
}

NewCGI::NewCGI( t_config &conf ) : Execute(conf), MAX_SEC_TO_TIMEOUT(4), conf(conf)
{
    one_time_kill = false;
    socketfd = conf.request->getFd();
    // std::ostringstream ss;
    // ss << "temporary/" << socketfd << ".output";
    // filename = ss.str();
    pid = -1;
    conf.response_code = "200";
    conf.cgi = false;
    conf.autoindex = "off";
    // std::remove(filename.c_str());
    // ss.str("");
    // ss << "temporary/" << socketfd << ".input";
    // ifilename = ss.str();
    // std::ofstream  inputf(ifilename, std::ios::out | std::ios::trunc | std::ios::binary);
    // if ( ! inputf.is_open())
    //     conf.response_code = "500";
    // else
    //     inputf.write(conf.request->getRequestBody().c_str(), conf.request->getRequestBody().length());
    // if (inputf.fail())
    //     conf.response_code = "500";
    // inputf.close();
    timeout_start = std::time(NULL);
}

void NewCGI::checkExitedProcess( void )
{
    std::vector<int> to_remove;
    int status = 0;
    std::map<int, NewCGI*>::iterator it;
    for (it = active_procs.begin(); it != active_procs.end(); ++it)
    {

        NewCGI *cgi = it->second;
        cgi->timeout();
        pid_t pid = waitpid(cgi->getPid(), &status, WNOHANG);
        if (pid == -1 || pid == 0)
            continue ;
        if (WIFSTOPPED(status) || WIFCONTINUED(status))
            continue;
        if (WIFEXITED(status))
            status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            status = WTERMSIG(status);
        cgi->onProcessExit(status);
        active_procs[cgi->socketfd ] = NULL;
        delete cgi;
        to_remove.push_back(it->first);
    }
    for (size_t i=0; i < to_remove.size(); ++i)
        active_procs.erase(to_remove[i]);
    to_remove.clear();
    for (size_t i=0; i < pids_to_remove.size(); ++i)
    {
        pid_t pid = pids_to_remove[i];
        if (pid == -1)
            continue ;
        pid = waitpid(pid, &status, WNOHANG);
        if (pid == -1 || pid == 0)
            continue ;
        pids_to_remove.erase(pids_to_remove.begin() + i);
    }
}

void NewCGI::build( config &conf )
{
    remove(conf.socket_fd);
    NewCGI *cgi = new NewCGI(conf);
    active_procs[conf.request->getFd()] = cgi;
    if ( ! cgi->execute() || conf.response_code != "200")
    {
        active_procs.erase(conf.request->getFd());
        conf.response_code = "500";//fork failed
        Response tmp(conf);
        delete cgi;
        return ;
    }
}

void NewCGI::onProcessExit( int status )
{
    std::string data;
    char buffer[4096];
    while (true)
    {
        int d = read(fds[0], buffer, 4096);
        if (d == -1)
        {
            conf.response_code = "500";
            Response tmp(conf);
            return ;
        }
        if (d == 0)
            break ;
        data.append(buffer, d);
    }
    ParseCGIOutput tmp(status, data, conf);
}

bool NewCGI::execute( void)
{
    if (pipe(fds) == -1)
    {
        conf.response_code = "500";
        return false;
    }
    pid_t pid = fork();
    if (pid == -1)
        return false;
    else if (pid == 0)
    {
        child();
        std::string dir = conf.translated_path.substr(0, conf.translated_path.find_last_of('/'));
        if (chdir(dir.c_str()) == -1)
            std::exit(EXIT_FAILURE);
        execve(conf.location.getCgiExe().c_str(), getArgs(), getEnv());
        std::exit(EXIT_FAILURE);
        return false;
    }
    close(fds[1]);
    fds[1] = -1;
    this->pid = pid;
    return true;
}

void NewCGI::child( void )
{
    //stdout to parent
    if (dup2(fds[1], STDOUT_FILENO) == -1)
        std::exit(EXIT_FAILURE);
    if (close(fds[0]) == -1)
        std::exit(EXIT_FAILURE);
    if (close(fds[1]) == -1)
        std::exit(EXIT_FAILURE);
    
    //body to stdin
    
    if (pipe(fds) == -1)
        std::exit(EXIT_FAILURE);
    if (dup2(fds[0], STDIN_FILENO) == -1)
        std::exit(EXIT_FAILURE);
    if (close(fds[0]) == -1)
        std::exit(EXIT_FAILURE);

    const char *body = conf.request->getRequestBody().c_str();
    size_t len = conf.request->getRequestBody().length();
    if (write(fds[1], body, len) == -1)
        std::exit(EXIT_FAILURE);
    if (close(fds[1]) == -1)
        std::exit(EXIT_FAILURE);
}

void NewCGI::timeout( void )
{
    if (one_time_kill || pid == -1)
        return ;
    int64_t now = std::time(NULL) - timeout_start;
    if (now >= MAX_SEC_TO_TIMEOUT)
    {
        kill(pid, SIGKILL);
        one_time_kill = true;
    }
}

pid_t NewCGI::getPid( void )
{
    return pid;
}

void NewCGI::setPid( pid_t pid )
{
    this->pid = pid;
}

NewCGI::~NewCGI( void )
{
    close(fds[1]);
    close(fds[0]);
}