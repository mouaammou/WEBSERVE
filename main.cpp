/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:07:10 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/19 15:03:12 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/Server.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Usage: ./webserv [port]" << std::endl;
        return (1);
    }
    /*
        void    fillPollFds();//fill pollfd structure for server socket: pollfd structure for server socket
        void    pollEvents();//wait for events: poll() function
        void    acceptConnections();//check for events on server socket: accept() function
        void    receiveRequests();//receive requests from clients: recv() function
        void    sendResponses();//send responses to clients: send() function
        void    closefds();//close client sockets: close() function
    */
    signal(SIGPIPE, SIG_IGN);
    Server myserver(av[1]);
    myserver.startServer();
    myserver.fillPollFds();
    myserver.pollEvents();// here where the magic happens, the server will wait for events, and when it receives one, it will call the appropriate function
    myserver.closefds();
    return (0);
}
