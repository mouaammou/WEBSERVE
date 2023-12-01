/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 23:04:03 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 23:06:22 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpResponse.hpp"

HttpResponse::HttpResponse( void )
{
}

HttpResponse::HttpResponse( HttpResponse const &other )
{
	(void)other;
}

HttpResponse& HttpResponse::operator=( HttpResponse const &other )
{
	(void)other;
	return *this;
}

HttpResponse::~HttpResponse( void )
{
}