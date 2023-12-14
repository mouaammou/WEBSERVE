/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollServers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 23:00:09 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/14 18:35:00 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pollServers.hpp"

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
}

void	PollServers::bindServers()
{
	for(size_t i = 0; i < this->num_servers; i++)
	{	
		setServerConfigurations(i);
		this->http_servers[i] = new Server(this->servers_config[i]);
		this->servers_config[i].server_fd = this->http_servers[i]->listenForConnections();
		addFileDescriptor(this->servers_config[i].server_fd);
	}
}

void 				PollServers::initPoll()
{
	int timeout = 50000;
	int pollStatu;

	while (true)
	{
		pollStatu = poll(this->poll_Fds.data(), this->poll_Fds.size(), timeout);
		if (pollStatu == -1)
		{
			perror ("poll");
			exit (EXIT_FAILURE);
		}
		else if (pollStatu == 0)
			std::cout << "poll timeout" << std::endl;
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
						Server *server = this->witchServer(this->poll_Fds[i].fd);
						// server->httpClients[this->poll_Fds[i].fd]->receiveRequest();
						server->httpClients[this->poll_Fds[i].fd]->receiveRequest();
						this->poll_Fds[i].events = POLL_OUT;
					}
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
	fcntl(newFd.fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);//set server socket to non-blocking mode
	this->poll_Fds.push_back(newFd);
}

void	PollServers::removeFileDescriptor(int fd)
{ 
	for (size_t i = 0; i < this->poll_Fds.size(); i++)
	{
		if (this->poll_Fds[i].fd == fd)
		{
			std::cout << COLOR_RED "SERVER disconnected :=> " COLOR_RESET<< fd << std::endl;
			close(fd);
			this->poll_Fds.erase(this->poll_Fds.begin() + i);
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
	//add the new client to the map
	
	Server *server = this->getTheServer(serverfd);
	if (server)
		server->addClient(clientSocket);
	std::cout << COLOR_RED "New client connected :=> " COLOR_RESET<< clientSocket << std::endl;
}
