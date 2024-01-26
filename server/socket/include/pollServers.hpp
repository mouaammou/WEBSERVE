/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollServers.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:57:32 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/26 12:12:52 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "webserv.hpp"
#include "Server.hpp"

class Server;

class PollServers
{
	private:
		std::vector<struct pollfd> poll_Fds;//servers and clients

		std::vector<Server*> http_servers;//severs
		std::vector <t_config> servers_config;

		t_config tmp_config;
		Config config_file;
		size_t num_servers;
		bool   multi_ports;

	public:
		PollServers(Config config_file);
		~PollServers();

		void 				initPoll();
		void 				bindServers();
		void				removeFromPoll(Server *server, int fd);
		void				addFileDescriptor(int fd);
		void				removeFileDescriptor(int fd);
		bool 				isServer(int fd);
		Server  			*whitchServer(int clientFd);
		Server  			*getTheServer(int fd);
		void				setServerConfigurations(int index);
		void   				acceptConnections(int serverfd);

		bool				setNewConfig(std::string &host, t_config &server_config);
		bool 				hasHostHeader(std::map<std::string, std::string> &headers, std::string &host_value);

		bool				IsConfigHasMultiPorts(void);

		bool				clientPollIn(Server *server, int fd);
		void				handleMultiPorts(Server *server, int fd);

		void	handleTranslatedPath(Server *server, int fd);
		void	handlePathInfo(Server *server, std::string path_info);

		void			    track_ALL_Clients(void);

		void					checkProxyMethod(Server *server, std::string re_method);

		bool		handle_Poll_Events(Server *server, int i, int fileDescriptor, Request *request);
		bool		handle_PollIn(Server *server, int i, int fileDescriptor, Request *HttpClient);
		bool		handle_PollOut(Server *server, int i, int fileDescriptor, Request *HttpClient);

		void 		handle_Method(Server *server, int fd);
};

Request				*TheClient(Server *server, int fd);
