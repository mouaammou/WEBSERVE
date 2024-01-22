/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseCGIOutput.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 17:57:35 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/22 14:51:27 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "Codes.hpp"
#include "Response.hpp"
#include "MediaTypes.hpp"
#include "Response.hpp"

class ParseCGIOutput
{
	private:

	std::string new_headers;
	std::string headers;
	std::string body;

	std::string	getFiled( std::string field );
	void	translateHeaders( void );
	void	generateStatusLine( void );
	void	additionalHeaders( void );

	public:
	
	ParseCGIOutput(int status,  std::string &output, config &args );
};
