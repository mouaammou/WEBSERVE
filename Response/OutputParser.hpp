/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   OutputParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 18:53:56 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/04 20:44:13 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <iostream>
# include <cstring>

class OutputParser
{
	public:

	static std::size_t buffsize;

	static std::string getLocalLocation( std::string header );
	static std::size_t getContentLength( std::string header );
	static std::string getStatusLine( std::string header );
	static bool thereIsContentLength( std::string header );
};