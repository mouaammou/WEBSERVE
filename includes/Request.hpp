/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mouaammo <mouaammo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 20:02:27 by mouaammo          #+#    #+#             */
/*   Updated: 2023/12/13 21:32:08 by mouaammo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../includes/Server.hpp"
#include "../includes/Method.hpp"
class Method;

class Request
{
	private:

		int fd;
		
		std::string requestString;
		std::string method;
		std::string path;
		std::string version;
		std::string requestBody;
		std::map<std::string, std::string> requestHeaders;
		size_t contentLength;
		std::string transferEncoding;
		bool _hasHeaders;
		bool _hasBody;

		std::string 	 statusCode;
		std::string 	responseString;
		t_config 		server_config;
		//Method, response
		Method* request_method;

	public:
		// Constructor to initialize the object with the raw HTTP request
		Request(int fd, t_config config_file);
		~Request();
		// Getters to retrieve information from the parsed request
		std::string 	getMethod() const;
		std::string 	 getPath() const;
		std::string 	  getVersion() const;
		std::string 	   getRequestBody() const;
		size_t			    getContentLength() const;
		int				     getFd() const;
		std::map<std::string, std::string>	getRequestHeaders() const;

		bool			  hasHeaders() const;
		bool			   hasBody() const;


		//display request headers
		void	displayRequest();
		// Function to parse the raw HTTP request
		void  	parseRequestFirstLine(const std::string& line);
		void   		parseRequestHeaders(const std::string& line);
		void    		storeRequestBody(std::stringstream& requestStream);


		void		checkMethod();
		void		 checkVersion();
		void		  checkPath();
		bool		   allowedURIchars(std::string& str);
		void		    checkRequestHeaders();
		
		
		void requestFormatError();
		void    resetRequestState();


		
		void parseRequest(std::string bufferString);
		bool   receiveRequest();
};
