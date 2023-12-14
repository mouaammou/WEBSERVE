/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollServers.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:57:32 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/14 03:29:35 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include "../includes/webserv.hpp"
#include "Server.hpp"
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
		void		addFileDescriptor(int fd);
		void		removeFileDescriptor(int fd);
		bool 				isServer(int fd);
		Server  			&witchServer(int clientFd);
		void				setServerConfigurations(int index);
		void   				acceptConnections(int serverfd);
};
