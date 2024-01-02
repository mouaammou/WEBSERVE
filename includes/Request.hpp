/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:02:27 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/02 01:09:52 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Server.hpp"

class Request
{
	private:

		int fd;
		
		std::string 	request_string;
		std::string 	method;
		std::string 	path;
		std::string 	version;
		std::string 	request_body;
		std::map<std::string, std::string> request_headers;

		size_t			content_length;
		std::string 	transfer_encoding;
		std::string 	content_type;
		std::string     query_string;
		
		bool	_has_headers;
		bool	_has_body;
		bool	request_received;
		int		read_bytes;
		char	*buffer;
		int		_body_size;
		
		std::string 	 _status_code;
		std::string 	response_string;
		t_config 		server_config;
		//Method, response

	public:
		// Constructor to initialize the object with the raw HTTP request
		Request(int fd, t_config config_file);
		~Request();
		// Getters to retrieve information from the parsed request
		std::string 	getMethod() const;
		int				getReadBytes() const;
		std::string 	 getPath() const;
		std::string 	  getVersion() const;
		std::string 	   getRequestBody() const;
		size_t			    getContentLength() const;
		std::string 		 getTransferEncoding() const;
		int				      getFd() const;
		std::string 		   getStatusCode() const;
		std::string 			 getQueryString() const;
		// Method*				    getRequestedMethod() const;
		std::map<std::string, std::string>&	getRequestHeaders();

		bool			  hasHeaders() const;
		bool			   hasBody() const;
		bool				hasRequest() const;
		void				 setRequestReceived(bool request_received);
		bool			      handleBadRequest();
		bool 				   isLocationHasRedirection();

		//display request headers
		void	displayRequest();
		// Function to parse the raw HTTP request
		bool  	parseRequestFirstLine(const std::string& line);
		bool   		parseRequestHeaders(const std::string& line);
		bool    		storeRequestBody();
		bool			 storeChunkedRequestBody();

		bool		checkMethod();
		bool		 checkVersion();
		bool		  checkPath();
		bool		   allowedURIchars(std::string& str);
		
		void    resetRequestState();
			
		bool	handleRequestHeaders(std::string buffe1rString);
		bool	receiveRequest();
		bool	sendResponse();
		int		get_file_size(int fd);
};
