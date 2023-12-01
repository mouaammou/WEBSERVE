/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 23:03:27 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 23:03:55 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>

class HttpResponse
{
	private:
	public:
		HttpResponse( void );
		HttpResponse( HttpResponse const &other );
		HttpResponse& operator=( HttpResponse const &other );
		~HttpResponse( void );
};
