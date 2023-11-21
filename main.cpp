/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/18 18:07:10 by mouaammo          #+#    #+#             */
/*   Updated: 2023/11/21 23:38:20 by mouaammo         ###   ########.fr       */
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
    Server myserver(av[1]);
    myserver.pollEvents();// here where the magic happens, the server will wait for events, and when it receives one, it will call the appropriate function
    myserver.closefds();
    return (0);
}
