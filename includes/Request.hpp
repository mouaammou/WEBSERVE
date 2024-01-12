/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:02:27 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/12 12:15:46 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Server.hpp"
#include <cstddef>
#include <sstream>
#include <string>

class Request
{
	private:

		int fd;
		
		std::string 	request_string;
		std::string 	method;
		std::string 	path;
		std::string 	version;
		std::map<std::string, std::string> request_headers;
		std::string 						request_body;

		size_t			content_length;
		std::string 	transfer_encoding;
		std::string 	content_type;
		std::string     query_string;//?name=ferret&color=purple

		bool	_has_headers;
		bool	_has_body;
		bool	request_received;
		long long		_body_size;


		std::string 	 _status_code;

	public:
		size_t reqeust_timeout;
		std::string      _connection;
		int   read_bytes;
		
		t_config 		server_config;
		char	*buffer;
		// Constructor to initialize the object with the raw HTTP request
		Request(int fd, t_config config_file);
		~Request();
		// Getters to retrieve information from the parsed request
		std::string 	getMethod() const;
		std::string 	 getPath() const;
		std::string 	  getVersion() const;
		std::string 	   &getRequestBody();
		size_t			    getContentLength() const;
		std::string 		 getTransferEncoding() const;
		std::string 			getContentType() const;
		int				      getFd() const;
		void					 setFd(int fd);
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
		bool   					 handleRequestBody();

		//display request headers
		void	displayRequest();
		// Function to parse the raw HTTP request
		bool  	parseRequestFirstLine(const std::string& line);
		bool   		parseRequestHeaders(const std::string& line);
		bool    		storeRequestBody();
		bool			 storeChunkedRequestBody();
		void				handlePathInfo();
		void				  handleQueryString();

		bool		checkMethod();
		bool		 checkVersion();
		bool		  checkPath();
		bool		   	allowedURIchars(std::string& str);
		bool 				checkEssentialHeaders(const std::map<std::string, std::string>& request_headers);
			
		bool	handleRequestHeaders(std::string buffe1rString);
		bool	receiveRequest();
		bool	sendResponse();

		void	resetRequest();

		std::string			extractChunks(const std::string& request);
};
