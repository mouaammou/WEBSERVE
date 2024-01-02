/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollServers.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:57:32 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/02 16:27:58 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../includes/webserv.hpp"
#include "../includes/Server.hpp"

class Server;

class PollServers
{
	private:
		std::vector<struct pollfd> poll_Fds;//servers and clients

		std::vector<Server*> http_servers;//severs
		std::vector <t_config> servers_config;

		Config config_file;
		size_t num_servers;

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

		bool				getServerHost(std::string &host, t_config &server_config);
		bool 				hasHostHeader(std::map<std::string, std::string> &headers, std::string &host_value);

		bool				IsConfigHasMultiPorts(void);

		bool				clientPollIn(Server *server, int fd);

		void			    trackALLClients(void);
};

void				stringTrim(std::string &str);
Request				*TheClient(Server *server, int fd);
