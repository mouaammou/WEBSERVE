/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PollServers.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 23:00:09 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/09 23:24:25 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pollServers.hpp"
#include "../Response/include/Response.hpp"

PollServers::PollServers(Config config_file)
{
	this->config_file = config_file;
	this->num_servers = config_file.get_directives().size();
	this->http_servers.resize(this->num_servers);
	this->servers_config.resize(this->num_servers);
}

PollServers::~PollServers()
{
	for (size_t i = 0; i < this->num_servers; i++)
	{
		delete this->http_servers[i];
		this->http_servers[i] = NULL;	
	}
}

void				PollServers::setServerConfigurations(int i)
{
	std::stringstream ss;
	ss << this->config_file.get_directives()[i].getPorts()[0];
	ss >> this->servers_config[i].port;
	this->servers_config[i].server_locations = this->config_file.get_directives()[i].getLocations();
	this->servers_config[i].server_name = this->config_file.get_directives()[i].getServerName();
	this->servers_config[i].body_size = this->config_file.get_directives()[i].getBodySize();
	this->servers_config[i].Server = &this->config_file.get_directives()[i];
}

void	PollServers::bindServers()
{
	for(size_t i = 0; i < this->num_servers; i++)
	{
		setServerConfigurations(i);
		this->http_servers[i] = new Server(this->servers_config[i]);
		this->servers_config[i].server_fd = this->http_servers[i]->listenForConnections();//listen, bind, socket
		this->http_servers[i]->setConfiguration(servers_config[i]);
		addFileDescriptor(this->servers_config[i].server_fd);

		std::cout << COLOR_GREEN "SERVER listening on port :=> " COLOR_RESET<< this->servers_config[i].port << std::endl;
	}
}

void			  PollServers::trackALLClients(void)
{
	Server		*server;
	int			fileDescriptor;

	for (size_t i = 0; i < this->poll_Fds.size(); i++)
	{
		if (this->poll_Fds[i].revents & POLLIN)
		{
			fileDescriptor = this->poll_Fds[i].fd;
			if (this->isServer(fileDescriptor))
				this->acceptConnections(fileDescriptor);
			else
			{
				server = this->whitchServer(fileDescriptor);
				if (server && !clientPollIn(server, fileDescriptor))//read <= 0, recieve request body
					continue;
			}
		}
		else
		{
			fileDescriptor = this->poll_Fds[i].fd;
			server = this->whitchServer(fileDescriptor);
			if (server && (this->poll_Fds[i].revents & POLLOUT) && TheClient(server, fileDescriptor)->hasRequest())//here
			{
				if (TheClient(server, fileDescriptor)->sendResponse())
				{
					TheClient(server, fileDescriptor)->resetRequestState();
					std::cout << COLOR_GREEN "response sent to client :=> " COLOR_RESET<< fileDescriptor << std::endl;
				}
			}
			// for (size_t i = 0; i < this->num_servers; i++)
			// {
			// 	this->http_servers[i]->setConfiguration(this->tmp_config[i]);
			// }
		}
		if (this->poll_Fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			removeFromPoll(server, this->poll_Fds[i].fd);
	}
}

void 				PollServers::initPoll()
{
	int timeout = 1000 * 60;
	int pollStatu;
	this->bindServers();//linten, bind, socket, non-blocking mode
	this->tmp_config = this->servers_config;
	while (true)
	{
		pollStatu = poll(this->poll_Fds.data(), this->poll_Fds.size(), timeout);
		if (pollStatu == -1)
		{
			perror ("poll");
			continue;
		}
		else if (pollStatu == 0)
			std::cout << COLOR_YELLOW "waiting for connections ..." COLOR_RESET<< std::endl;
		else
		{
			this->trackALLClients();
		}
		CGI::checkTimeoutAndExitedProcesses();
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

Server*				PollServers::whitchServer(int clientFd)
{
	for (size_t i = 0; i < this->num_servers; i++)
	{
		if (this->http_servers[i] && this->http_servers[i]->isClient(clientFd))
			return (this->http_servers[i]);
	}
	return (NULL);
}

void				PollServers::removeFromPoll(Server *server ,int fd)
{
	std::cout << COLOR_RED "Client removed " << fd << COLOR_RESET << std::endl;
	this->removeFileDescriptor(fd);
	if (server)
		server->removeClient(fd);
	CGI::remove(fd);
}

void	PollServers::addFileDescriptor(int fd)
{
	struct pollfd newFd;

	newFd.fd = fd;
	newFd.events = POLLIN | POLLOUT;//read and write allowed
	fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC);//set server socket to non-blocking mode
	this->poll_Fds.push_back(newFd);
}

void		PollServers::removeFileDescriptor(int fd)
{
	for (size_t i = 0; i < this->poll_Fds.size(); i++)
	{
		if (this->poll_Fds[i].fd == fd)
		{
			this->poll_Fds.erase(this->poll_Fds.begin() + i);
			close(fd);
			break;
		}
	}
}

void		PollServers::acceptConnections(int serverfd)
{
    struct sockaddr		clientAddr;
    int					clientSocket;

    socklen_t clientAddrLen = sizeof(clientAddr);
    if ((clientSocket = accept(serverfd, &clientAddr, &clientAddrLen)) == -1)
    {
        perror("accept");
        return;
    }
	addFileDescriptor(clientSocket);
	std::cout << COLOR_YELLOW "New client connected :=> " COLOR_RESET<< clientSocket << std::endl;
	Server *server = this->getTheServer(serverfd);
	if (server)
		server->addClient(clientSocket);
}

Request				*TheClient(Server *server, int fd)
{
	if (server && server->httpClients.find(fd) != server->httpClients.end())
		return (server->httpClients[fd]);
	return (NULL);
}

void				stringTrim(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
	{
		if (str[i] == '\r' || str[i] == '\n' || str[i] == '\t' || str[i] == ' ')
		{
			str.erase(i, 1);
			i--;
		}
	}
}

bool				PollServers::setNewConfig(std::string &host, t_config &server_config)
{
	for(size_t i = 0; i < this->num_servers; i++)
	{
		stringTrim(host);
		if (this->http_servers[i]->serverConfigFile.server_name == host)
		{
			server_config = this->http_servers[i]->serverConfigFile;
			return (true);
		}
	}
	return (false);
}

bool 				PollServers::hasHostHeader(std::map<std::string, std::string> &headers, std::string &host_value)
{
	for(std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
	{
		if (it->first == "Host:")
		{
			host_value = it->second;
			return (true);
		}
	}
	return (false);
}

bool				PollServers::IsConfigHasMultiPorts(void)
{
	for(size_t i = 0; i < this->num_servers; i++)
	{
		for(size_t j = 0; j < this->num_servers; j++)
		{
			if (this->http_servers[i]->serverConfigFile.port == this->http_servers[j]->serverConfigFile.port && i != j)
				return (true);
		}
	}
	return (false);
}

bool				PollServers::clientPollIn(Server *server, int fd)
{
	if (TheClient(server, fd)->receiveRequest())//status code generated
	{
		TheClient(server, fd)->setRequestReceived(true);
		
		std::string host_value;
		if (IsConfigHasMultiPorts())
		{
			if (hasHostHeader(TheClient(server, fd)->getRequestHeaders(), host_value))
				setNewConfig(host_value, server->serverConfigFile);
		}
		// printf("content type: %s\n", TheClient(server, fd)->getContentType().c_str());
		TheClient(server, fd)->displayRequest();

		server->setStatusCode(TheClient(server, fd)->getStatusCode());
		std::string path = TheClient(server, fd)->getPath();
		std::string re_location = server->getRequestedLocation(path);

		server->serverConfigFile.translated_path 	= server->getTranslatedPath(re_location, path);
		server->serverConfigFile.requested_path 	= path;
		server->serverConfigFile.request 			= TheClient(server, fd);

		//check allowed methods in config file here:
		// TheClient(server, fd)->displayRequest();

		if (server->getStatusCode().find("200") != std::string::npos)
		{
			if (TheClient(server, fd)->getMethod() == "GET")
			{
				server->pointedMethod = new Method(server->serverConfigFile);
			}
			else if (TheClient(server, fd)->getMethod() == "DELETE")
			{
				server->pointedMethod = new Method(server->serverConfigFile, 1337);
			}
			else if (TheClient(server, fd)->getMethod() == "POST")
			{
				server->pointedMethod = new Method(server->serverConfigFile, "post");
			}
			server->printf_t_config(server->serverConfigFile);
			// delete server->pointedMethod;
			// server->pointedMethod = NULL;

		}
		Response response(server->serverConfigFile);
		// for (size_t i = 0; i < this->num_servers; i++)
		// {
		// 	this->http_servers[i]->setConfiguration(this->tmp_config[i]);
		// 	if (host_value == this->http_servers[i]->serverConfigFile.server_name)
		// 		this->http_servers[i]->serverConfigFile.request = TheClient(server, fd);
		// }
	}
	else if (TheClient(server, fd)->getReadBytes() <= 0)
		return (removeFromPoll(server, fd), false);
	else
		return (false);
	return true;
}
