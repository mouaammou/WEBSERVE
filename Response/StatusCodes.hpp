/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   StatusCodes.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/01 20:35:20 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/01 20:37:02 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once
# include <string>
# include <map>

// https://datatracker.ietf.org/doc/html/rfc"261",6#section-6
// HTTP/1.1

class StatusCodes
{
	private:

	static std::string					_scodes[40][2];
	std::map<std::string, std::string>	codes;

	public:

	StatusCodes( void );
	std::string getStatusLine( std::string code );
};