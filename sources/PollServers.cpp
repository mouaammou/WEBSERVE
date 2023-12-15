/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollServers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 23:00:09 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/15 18:19:20 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/PollServers.hpp"
#include <string>
#include <sys/poll.h>

PollServers::PollServers(Config config_file)
{
	this->config_file = config_file;
	this->num_servers = config_file.get_directives().size();
	this->http_servers.resize(this->num_servers);
	this->servers_config.resize(this->num_servers);
}

PollServers::~PollServers()
{
}

void				PollServers::setServerConfigurations(int i)
{
	this->servers_config[i].server_locations = this->config_file.get_directives()[i].getLocations();
	this->servers_config[i].server_name = this->config_file.get_directives()[i].getServerName();
	this->servers_config[i].port = std::to_string(this->config_file.get_directives()[i].getPorts()[0]);
}

void	PollServers::bindServers()
{
	for(size_t i = 0; i < this->num_servers; i++)
	{	
		setServerConfigurations(i);
		this->http_servers[i] = new Server(this->servers_config[i]);
		this->servers_config[i].server_fd = this->http_servers[i]->listenForConnections();
		addFileDescriptor(this->servers_config[i].server_fd);
		std::cout << COLOR_GREEN "SERVER listening on port :=> " COLOR_RESET<< this->servers_config[i].port << std::endl;
	}
}

void 				PollServers::initPoll()
{
	int timeout = 5000;
	int pollStatu;
	Server *server = NULL;
	puts("init poll");
	this->bindServers();
	while (true)
	{
		pollStatu = poll(this->poll_Fds.data(), this->poll_Fds.size(), timeout);
		if (pollStatu == -1)
		{
			perror ("poll");
			exit (EXIT_FAILURE);
		}
		else if (pollStatu == 0)
			std::cout << COLOR_YELLOW "waiting for connections ..." COLOR_RESET<< std::endl;
		else
		{
			for (size_t i = 0; i < this->poll_Fds.size(); i++)
			{
				if (this->poll_Fds[i].revents & POLLIN)
				{
					if (this->isServer(this->poll_Fds[i].fd))
						this->acceptConnections(this->poll_Fds[i].fd);
					else
					{
						server = this->witchServer(this->poll_Fds[i].fd);
						if (server->httpClients[this->poll_Fds[i].fd]->receiveRequest())
						{
							server->httpClients[this->poll_Fds[i].fd]->resetRequestState();
							server->httpClients[this->poll_Fds[i].fd]->displayRequest();
							this->poll_Fds[i].events = POLLOUT;
							continue;
						}
					}
				}
				else if (this->poll_Fds[i].revents & POLLOUT)
				{
					std::cout << COLOR_GREEN "sending response to client :=> " COLOR_RESET<< this->poll_Fds[i].fd << std::endl;
					server = this->witchServer(this->poll_Fds[i].fd);
					if (server->httpClients[this->poll_Fds[i].fd]->sendResponse())
						this->poll_Fds[i].events = POLL_IN;
				}
				if (this->poll_Fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				{
					int fd = this->poll_Fds[i].fd;
					this->removeFileDescriptor(this->poll_Fds[i].fd);
					printf("vector size = %zu\n", this->poll_Fds.size());
					if (server)
						server->removeClient(fd);
				}
			}
		}
	}
}

bool 				PollServers::isServer(int fd)
{
	for (size_t i = 0; i < this->num_servers; i++)
	{
		if (this->servers_config[i].server_fd == fd)
			return (true);
	}
	return (false);
}

Server*				PollServers::getTheServer(int fd)
{
	for (size_t i = 0; i < this->num_servers; i++)
	{
		if (this->http_servers[i]->getServerSocket() == fd)
			return (this->http_servers[i]);
	}
	return (NULL);
}

Server*				PollServers::witchServer(int clientFd)
{
	for (size_t i = 0; i < this->num_servers; i++)
	{
		if (this->http_servers[i]->isClient(clientFd))
			return (this->http_servers[i]);
	}
	return (NULL);
}

void	PollServers::addFileDescriptor(int fd)
{
	struct pollfd newFd;

	newFd.fd = fd;
	newFd.events = POLL_IN;
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);//set server socket to non-blocking mode
	this->poll_Fds.push_back(newFd);
}

void	PollServers::removeFileDescriptor(int fd)
{ 
	for (size_t i = 0; i < this->poll_Fds.size(); i++)
	{
		if (this->poll_Fds[i].fd == fd)
		{
			std::cout << COLOR_RED << "removed fd: "<< fd << COLOR_RESET << std::endl;
			this->poll_Fds.erase(this->poll_Fds.begin() + i);
			close(fd);
			break;
		}
	}
}

void   PollServers::acceptConnections(int serverfd)
{
    struct sockaddr clientAddr;
    int clientSocket;
    socklen_t clientAddrLen = sizeof(clientAddr);

    if ((clientSocket = accept(serverfd, &clientAddr, &clientAddrLen)) == -1)
    {
        perror("accept");
        return;
    }
    //add the new client socket to the pollfd array
	addFileDescriptor(clientSocket);
	std::cout << COLOR_YELLOW "New client connected :=> " COLOR_RESET<< clientSocket << std::endl;
	//add the new client to the map
	
	Server *server = this->getTheServer(serverfd);
	if (server)
		server->addClient(clientSocket);
}
