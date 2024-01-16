/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 02:03:32 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/16 02:31:03 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/Cookies.hpp"

void  Session::setCookies(std::map<std::string, std::string> cookies)
{
    this->_cookies = cookies;
}

std::string      Session::generateSessionToken()
{
    std::string token;
    std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    srand(time(NULL));
    for (int i = 0; i < 32; i++)
        token += charset[rand() % charset.length()];
    return (token);
}

void        Session::createSession(std::string const & username)
{
    std::string sessionId = this->generateSessionToken();

    this->_cookies["webser-session_id"] = sessionId;
    this->_cookies["username"] = username;

    setCookies(this->_cookies);
}
