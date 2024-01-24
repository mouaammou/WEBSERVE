/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ParseCGIOutput.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 17:57:35 by samjaabo          #+#    #+#             */
/*   Updated: 2024/01/24 11:44:22 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include "../../Response/include/Codes.hpp"
#include "../../Response/include/Response.hpp"
#include "../../Response/include/MediaTypes.hpp"
#include "../../Response/include/Response.hpp"

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
