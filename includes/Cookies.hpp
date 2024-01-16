/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Cookies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/16 02:05:13 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/16 02:28:25 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "webserv.hpp"

class Session
{
    public:
        std::map<std::string, std::string> _cookies;
        Session();
        ~Session();
        void                                                setCookies(std::map<std::string, std::string> cookies);
        std::string                                         generateSessionToken();

        void        createSession(std::string const & token);
};