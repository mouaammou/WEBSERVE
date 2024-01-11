/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollServers.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:57:32 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/10 21:11:41 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../includes/webserv.hpp"
#include "../includes/Server.hpp"
#include "../Response/include/PipeStream.hpp"

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

		void			    trackALLClients(void);
};

void				stringTrim(std::string &str);
Request				*TheClient(Server *server, int fd);
