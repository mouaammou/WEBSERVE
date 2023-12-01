/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:45:10 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 23:06:28 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

HttpRequest::HttpRequest( void )
{
}

HttpRequest::HttpRequest( HttpRequest const &other )
{
	(void)other;
}

HttpRequest& HttpRequest::operator=( HttpRequest const &other )
{
	(void)other;
	return *this;
}

HttpRequest::~HttpRequest( void )
{
}
