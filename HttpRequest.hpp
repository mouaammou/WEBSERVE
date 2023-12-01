/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 22:45:07 by samjaabo          #+#    #+#             */
/*   Updated: 2023/11/20 17:52:45 by mouaammo         ###   ########.fr       */
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
