/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Codes.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:04:23 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/22 23:00:38 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <stdexcept>
#include <sstream>
#include <unistd.h>
#include <ctime>

class StatusCodes
{
	private:
	
	std::map<std::string, std::string>	codes;

	public:

	StatusCodes(void);

	const std::string getStatusLine( std::string const &code ) const;
	const std::string getStatusLine( int code ) const;
};