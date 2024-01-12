/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/30 23:57:01 by mouaammo          #+#    #+#             */
/*   Updated: 2024/01/12 01:22:10 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include <iterator>
#include "directives.hpp"
#define COLOR_RED     "\033[0;31m"
#define COLOR_RESET   "\033[0m"
#ifndef CONFIG_HPP
#define CONFIG_HPP


class Config
{
	private:
		static int count;
		int count_s;
		int open_brack;
		int close_brack;
		std::vector<std::string> ftokens;
		std::vector<Directives> directs;
	public:
		Config();
		std::vector<Directives>& get_directives();
		Config(std::string file);
		void tokenizer(std::string token);
		bool str_digit(std::string str);
		void remove_spces();
		void print_file();
		void print_file2(Config &conf);
		void server_count();
		void fill_directive();
		void error_page(std::vector<std::string> vec, int indice);
		void handle_servers(int *indice);
		void check_coma(std::string line);
		void error_call(std::string error);
		void handle_brackets(void);
		//server :
		void handle_port(Directives& server,std::vector<int> &ports, int *indice);
		std::string next_str_arg(int *indice);
		unsigned long long max_body_size(int *indice);
		void handle_error_page(Directives &server, int *indice);
		void handle_locations(int *indice);
		void handle_inside_locations(Directives& server, int *indice);
		int handle_autodx(int *indice);
		void handle_proxy_methode(Location &location, int *indice);
		bool is_token(int *indice);
		void check_server_name_dup(std::string serv_name);
		const std::string get_page_error(std::string serv_name, int status);
		void summarize();
		std::vector<Location> sort_location(std::vector<Location> locat);
		void check_dup_location(std::vector<Location> locat);
		void handle_redirection(Location &location, int *indice);
		unsigned long long int octet_convert(std::string& str);
		~Config();
};

#endif

typedef struct s_server_data
{
	Location my_location;
	std::string server_name;
	std::string host_name;
	std::vector<std::string> def_method;
	std::map<int, std::string> error_page;
	std::vector<int> port;
	int server_id;
	int body_size;
}server_data;
