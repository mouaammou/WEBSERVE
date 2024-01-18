/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NewCGI.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 04:01:07 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/18 01:06:20 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/NewCGI.hpp"


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

NewCGI::NewCGI( t_config &conf ) : Execute(conf), MAX_MSEC_TO_TIMEOUT(800), conf(conf)
{
    socketfd = conf.request->getFd();
    std::ostringstream ss;
    ss << "temporary/" << socketfd << ".output";
    filename = ss.str();
    pid = -1;
    timeout_start = getCurrentTime();
    conf.response_code = "200";
    conf.cgi = false;
    std::remove(filename.c_str());
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
    remove(conf.server_fd);
    NewCGI *cgi = new NewCGI(conf);
    active_procs[conf.request->getFd()] = cgi;
    if ( ! cgi->execute())
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
    std::ifstream file(filename.c_str());
    if ( ! file.is_open())
    {
        conf.response_code = "500";
        Response tmp(conf);
        return ;
    }
    std::string line;
    std::string data;

    while (std::getline(file, line))
    {
        data.append(line + "\n");
    }
    file.close();
    ParseCGIOutput tmp(status, data, conf);
}

bool NewCGI::execute( void)
{
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
    this->pid = pid;
    return true;
}

void NewCGI::child( void )
{
    int fd = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1)
        std::exit(EXIT_FAILURE);
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        close(fd);
        std::exit(EXIT_FAILURE);
    }
    close(fd);
    /*** put pipe in STDIN, And write request body to it ***/
    int fds[2];
    if (pipe(fds) == -1)
        std::exit(EXIT_FAILURE);
    if (dup2(fds[0], STDIN_FILENO) == -1)
    {
        close(fds[0]);
        close(fds[1]);
        std::exit(EXIT_FAILURE);
    }
    close(fds[0]);
    if (write(fds[1], conf.request->getRequestBody().c_str(), conf.request->getRequestBody().length()) == -1)
    {
        close(STDIN_FILENO);
        close(fds[1]);
        std::exit(EXIT_FAILURE);
    }
    if (close(fds[1]) == -1)
        std::exit(EXIT_FAILURE);
}

int64_t NewCGI::getCurrentTime( void )
{
    struct timeval tv;
    if (gettimeofday(&tv, NULL))
        return (0);
    return (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
}

void NewCGI::timeout( void )
{
    static bool one_time_kill = false;
    if (one_time_kill || pid == -1)
        return ;
    int64_t now = getCurrentTime() - timeout_start;
    if (now >= MAX_MSEC_TO_TIMEOUT)
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