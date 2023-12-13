/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pollServers.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 20:57:32 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/13 21:49:15 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Server;

#include "../includes/Server.hpp"

class pollServers
{
	private:
		std::vector<struct pollfd> pollFds;//servers and clients
		std::map<int, Server*> servers;//severs
	public:
		pollServers();
		~pollServers();

		bool isServer(int fd);
		void addFileDescriptor(int fd, config *conf);
		Server  witchServer(int clientFd);
};
