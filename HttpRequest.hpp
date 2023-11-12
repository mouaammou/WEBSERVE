/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:45:07 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/12 22:47:35 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>

class HttpRequest
{
	private:
	public:
		HttpRequest( void );
		HttpRequest( HttpRequest const &other );
		HttpRequest& operator=( HttpRequest const &other );
		~HttpRequest( void );
};
