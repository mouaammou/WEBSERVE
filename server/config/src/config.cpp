/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 14:38:58 by moouaamm          #+#    #+#             */
/*   Updated: 2024/01/30 13:49:06 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/config.hpp"

int Config::count = 0;

Config::Config(/* args */)
{
}

std::vector<Directives>& Config::get_directives()
{
	return this->directs;
}


void Config::check_coma(std::string line)
{
	int i;
	int size;

	size = line.size();
	if (line == "server" || line.find("location") != std::string::npos)
		return ;
	i = 1;
	while (line[size - i] == ' ' || line[size - i] == '\t')
		i++;
	if (i == size + 1 || line[size - 1] == '{' || line[size - 1] == '}')
		return;
	if (line[size - i] != ';')
	{
		std::cout << line << std::endl;
		error_call("semi-colone ';' not in the end of the line!");
	}
}

Config::Config(std::string file)
{
	std::ifstream tmp_infile(file);
	std::string line;
	std::string comm_line;
	int pos;
	if (!tmp_infile.is_open())
		std::cout << "Failed to open config file" << std::endl;
	while (getline(tmp_infile, comm_line))
	{
		pos = comm_line.find("#");
		if (pos != -1)
			line = comm_line.substr(0, pos);
		else
			line = comm_line;
		check_coma(line);
		this->ftokens.push_back(line);
	}
	if (this->ftokens.size() == 0)
		error_call("Config file is empty");
	this->open_brack = 0;
	this->close_brack = 0;
	tmp_infile.close();
}

void Config::print_file()
{
	// std::cout << this->infile << std::endl;
	unsigned int i = 0;
	while (i < this->ftokens.size())
	{
		std::cout << "--------------------------------\n";
		std::cout << this->ftokens[i] << std::endl;
		i++;
	}
	std::cout << this->ftokens.size() << std::endl;
}

void Config::handle_brackets(void)
{
	std::string brackets;
	for (size_t i = 0; i < this->ftokens.size(); i++)
	{
		if (this->ftokens[i] == "{" && i  + 1 < this->ftokens.size() && this->ftokens[i + 1] == "}")
			error_call("brackets error, empty brackets are not accepted!");
		if (this->ftokens[i] == "{")
		{
			brackets += "{";
			this->open_brack++;
		}
		if (this->ftokens[i] == "}")
		{
			brackets += "}";
			this->close_brack++;
		}
		if (ftokens[i] == "server" && open_brack != close_brack)
			error_call("brackets error");
	}
	for (size_t i = 0; i < 2; i++)
	{
		size_t pos;
		pos = brackets.find("{}");
		while (pos != (size_t)std::string::npos)
		{
			brackets.erase(pos, 2);
			pos = brackets.find("{}", pos);
		}
	}
	if (brackets.size() != 0)
		error_call("brackets error");
	for (size_t i = 0; i < this->ftokens.size(); i++)
	{
		if (this->ftokens[i] == "{")
		{
			while (i + 1 < this->ftokens.size() && this->ftokens[i + 1] == ";")
				i++;
			if ( i + 1 < this->ftokens.size() && this->ftokens[i + 1] == "}")
				error_call("brackets error, empty brackets are not accepted!");
		}
	}
}


void Config::tokenizer(std::string token)
{
	size_t pos;
	size_t last_pos;
	std::vector<std::string> tmp_vec;

	for (size_t i = 0; i < ftokens.size(); i++)
	{
		pos = 0;
		last_pos = 0;
		while ((pos = ftokens[i].find(token, pos)) != std::string::npos)
		{
			tmp_vec.push_back(ftokens[i].substr(last_pos, pos - last_pos)); // Adjusted substring range
			tmp_vec.push_back(token);
			pos++;
			last_pos = pos;
		}
		tmp_vec.push_back(ftokens[i].substr(last_pos));
	}
	this->ftokens.clear();
	this->ftokens = tmp_vec;
}

void Config::remove_spces()
{
	std::vector<std::string> vec;
	std::string str;
	for (size_t i = 0; i < this->ftokens.size(); i++)
	{
		std::istringstream space(ftokens[i]);
		while (space >> str)
		{
			vec.push_back(str);
		}
	}
	this->ftokens.clear();
	this->ftokens = vec;
	server_count();
	this->directs.reserve(this->count_s);
}

bool Config::str_digit(std::string str)
{
	size_t i = 0;
	if (!str.empty() && str[0] == '-')
		error_call("negative numbers are not accepted!");
	while (i < str.size())
	{
		if (!isdigit(str[i]))
			return false;
		i++;
	}
	return true;
}



void Config::server_count()
{
	size_t i ;
	this->count_s = 0;
	i = 0;
	while (i < this->ftokens.size())
	{
		if (this->ftokens[i] == "server")
			this->count_s++;
		i++;
	}
}

void Config::error_call(std::string error)
{
	for (size_t i = 0; i < error.size(); i++)
		error[i] = toupper(error[i]);
	std::cerr << "\033[0;31m" << error << "\033[0m" << std::endl;
	exit(1);
}

int Config::handle_autodx(int *indice)
{
	int size;
	size = this->ftokens.size();
	if (size == (*indice) + 1)
		error_call("auto index not set!");
	if (this->ftokens[(*indice) + 1] == ";")
		error_call("auto index not set!");
	(*indice)++;
	if (ftokens[(*indice)] == "on")
		return 1;
	else if (ftokens[(*indice)] == "off")
		return 0;
	else
		error_call("auto index take only on or off!");
	if (this->ftokens[(*indice) + 1].compare(";"))
		error_call("auto index accept only one attribute");
	return 0;
}

bool methode_duplicated(std::vector<std::string> methods)
{
	int get;
	int post;
	int dele;
	get = post = dele = 0;
	for (size_t i = 0; i < methods.size(); i++)
	{
		if (methods[i] == "GET")
			get++;
		else if (methods[i] == "POST")
			post++;
		else
			dele++;
	}
	if (get > 1 || post > 1 || dele > 1)
		return true;
	return false;
}

void Config::handle_redirection(Location &location, int *indice)
{
	int tmp;
	tmp = (*indice);
	while (ftokens[tmp].compare(";"))
		tmp++;
	if (tmp - *indice != 3)
		error_call("error in redirection of locatioin : " + location.getName());
	(*indice)++;
	location.set_return_status(std::stoi(ftokens[*indice]));
	location.setReturnInt(1);
	(*indice)++;
	location.setReturnString(ftokens[*indice]);
}

void Config::handle_proxy_methode(Location &location, int *indice)
{
	int tmp;
	std::vector<std::string> methods;
	tmp = (*indice);
	while (ftokens[tmp].compare(";"))
		tmp++;
	(*indice)++;
	if ((*indice) == tmp || tmp - (*indice) > 3)
		error_call("error proxy_method!");
	while ((*indice) < tmp)
	{
		if (ftokens[(*indice)] == "GET")
			methods.push_back(ftokens[(*indice)]);
		else if (ftokens[(*indice)] == "POST")
			methods.push_back(ftokens[(*indice)]);
		else if (ftokens[(*indice)] == "DELETE")
			methods.push_back(ftokens[(*indice)]);
		else
			error_call("only GET,POST and DELETE accepted!");
		(*indice)++;
	}
	if (methods.size() > 3 || methode_duplicated(methods))
		error_call("methods must not be duplicated!");
	location.setMethods(methods);
}


void Config::handle_inside_locations(Directives& server, int *indice)
{
	Location locat(ftokens[*indice]);
	int retur, autoindex, upload_path, exe, index, proxy_method, root;
	retur = autoindex = upload_path = exe = index = proxy_method = root = 0;
	locat.setReturnInt(0);
	(*indice)++;
	while (ftokens[*indice].compare("}") && *indice < (int)ftokens.size())
	{
		if (ftokens[*indice] == "root")
		{
			locat.setRoot(next_str_arg(indice));
			root++;
		}
		else if (ftokens[*indice] == "return")
		{
			handle_redirection(locat, indice);
			retur++;
		}
		else if (ftokens[*indice] == "autoindex")
		{
			locat.setAutoindex(handle_autodx(indice));
			autoindex++;
		}
		else if (ftokens[*indice] == "upload_path")
		{
			locat.setUploadPath(next_str_arg(indice));
			upload_path++;
		}
		else if (ftokens[*indice] == "exe")
		{
			locat.setCgiExe(next_str_arg(indice));
			exe++;
		}
		else if (ftokens[*indice] == "proxy_method")
		{
			handle_proxy_methode(locat, indice);
			proxy_method++;
		}
		else if (ftokens[*indice] == "index")
		{
			locat.setIndex(next_str_arg(indice));
			index++;
		}
		else if (!is_token(indice))
			error_call(ftokens[*indice] + " not refer to any arg inside location!");
		(*indice)++;
	}
	if (!locat.getReturnInt() && !root)
		error_call("root or redirection must be present in location section : " + locat.getName());
	if (retur > 1 || autoindex > 1 || upload_path > 1 || exe > 1 || index > 1 || proxy_method > 1)
		error_call("directive must not be duplicated!");
	if ( ! proxy_method )
		error_call("proxy_method must be set for any location!");
	if (!locat.getReturnInt() && !autoindex && !index  )
		error_call("autoindex or index must be set in location : " + locat.getName());
	server.server_locations.push_back(locat);
}

void Config::handle_locations(int *indice)
{
	int size;
	int tmp;
	size = this->ftokens.size();
	if (size == *indice + 1)
		error_call("location not set!");
	tmp = *indice;
	while (ftokens[tmp].compare("}"))
		tmp++;
	if (tmp - *indice < 3)
		error_call("location not set!");
	(*indice)++;
	if (ftokens[*indice] == "{" || ftokens[*indice + 1].compare("{"))
		error_call("error in path of location!");
}

bool containsElement(const std::vector<int>& vec, int element)
{
	for (size_t i = 0; i < vec.size(); i++)
	{
		if (vec[i] == element)
			return true;
	}
	return false;
}


void Config::handle_port(Directives& server,std::vector<int> &ports, int *indice)
{
	(void)server;
	int size;
	size = this->ftokens.size();
	if (size == *indice + 1)
		error_call("port not set!");
	if (this->ftokens[*indice + 1] == ";")
		error_call("port not set!");
	(*indice)++;
	if (this->ftokens[*indice + 1].compare(";"))
		error_call("port accept only one attribute!");
	if (!str_digit(ftokens[*indice]))
		error_call("port must be digit!");
	int port;
	port = atoi(ftokens[*indice].c_str());
	if (port <= 0 || port > 65535)
		error_call("use a port between 1 and 65535!");
	if (containsElement(ports, port))
		error_call("port is duplicated!");
	ports.push_back(port);
}

std::string Config::next_str_arg(int *indice)
{
	int size;
	size = this->ftokens.size();
	if (size == *indice + 1)
		error_call(this->ftokens[*indice] + " not set!");
	if (this->ftokens[*indice + 1] == ";")
		error_call(this->ftokens[*indice] + " not set!");
	(*indice)++;
	if (this->ftokens[*indice + 1].compare(";"))
		error_call(this->ftokens[*indice - 1] + " directive accept only one attribute");
	return(this->ftokens[*indice]);
}

unsigned long long int Config::octet_convert(std::string& str)
{
	std::stringstream ss;
	unsigned long long value;
	ss << str;
	ss >> value;//1000
	switch (std::toupper(str[str.length() - 1]))
	{
		case 'K':
			return value * 1024;
		case 'M':
			return value * 1024 * 1024;
		case 'G':
			return value * 1024 * 1024 * 1024;
		default:
			return value;
	}
	return (0);
}


unsigned long long Config::max_body_size(int *indice)
{
	int size;
	unsigned long long max;
	size = this->ftokens.size();
	if (size == *indice + 1)
		error_call(this->ftokens[*indice] + " not set!");
	if (this->ftokens[*indice + 1] == ";")
		error_call(this->ftokens[*indice] + " not set!");
	(*indice)++;
	if (this->ftokens[*indice + 1].compare(";"))
		error_call(this->ftokens[*indice - 1] + " directive accept only one attribute");
	if (this->ftokens[*indice + 1].size() > 18)
		error_call(this->ftokens[*indice] + " is too large!");
	if (!str_digit(ftokens[*indice]))
	{
		if (ftokens[*indice].find("M", 0) != std::string::npos || ftokens[*indice].find("K", 0) != std::string::npos
				 || ftokens[*indice].find("G", 0) != std::string::npos)
			return(octet_convert(ftokens[*indice]));
		else
			error_call("max body  size must be digits!");
	}
	std::stringstream ss(ftokens[*indice]);
	ss >> max;
	return(max);
}

bool valid_status(int status)
{
	if ((status >= 100 && status <= 101) || (status >= 200 && status <= 206))
		return true;
	if ((status >= 300 && status <= 307) || (status >= 400 && status <= 417))
		return true;
	if ((status >= 500 && status <= 505))
		return true;
	return false;
}

void Config::handle_error_page(Directives &server, int *indice)
{
	int size;
	size_t tmp;
	std::string path;
	int status;
	size = this->ftokens.size();
	if (size == *indice + 1)
		error_call(this->ftokens[*indice] + " not set!");
	if (this->ftokens[*indice + 1] == ";")
		error_call(this->ftokens[*indice] + " not set!");
	(*indice)++;
	tmp = *indice;
	while (tmp < this->ftokens.size() && ftokens[tmp].compare(";"))
		tmp++;
	tmp--;
	if ((size_t)*indice == tmp)
		error_call("after error_page there must be a valid status and path");
	path = ftokens[tmp];
	while ((size_t)*indice < tmp)
	{
		if (!str_digit(ftokens[*indice]))
		{
			std::cout << ftokens[*indice] << std::endl;
			error_call("error page status must be digits");
		}
		status = atol(ftokens[*indice].c_str());
		if (!valid_status(status))
			error_call("status not between 1XX and 5XX");
		server.setErrorPage(status, path);
		(*indice)++;
	}
}



void Config::check_server_name_dup(std::string serv_name)
{
	for (size_t i = 0; i < this->directs.size(); i++)
	{
		if (directs[i].getServerName() == serv_name)
			error_call(serv_name + " is already used to name previous server!");
	}
}

int Config::stop_indice(int indice)
{
	int tmp;
	for (size_t i = indice; i < ftokens.size(); i++)
	{
		if (ftokens[i] == "}" && i + 1 < ftokens.size() && ftokens[i + 1] == "}")
		{
			if (i + 2 < ftokens.size() && ftokens[i + 2].compare("server"))
			{
				error_call("error! " + ftokens[i + 2] + " is outside of the server!");
			}
			return i + 1;
		}
		tmp = i;
	}
	return tmp;
}

void Config::handle_servers(int *indice)
{
	Directives server;
	std::vector<int> ports;
	int size;
	int port = 0;
	int srv_name = 0;
	int body_size = 0;
	int host_name = 0;
	size = this->ftokens.size();
	if (size == *indice + 1)
		error_call("server not set!");
	if (this->ftokens[*indice + 1] != "{")
		error_call("brackets { not open after server!");
	(*indice)++;
	this->count++;
	server.setServerId(this->count);
	server.setBodySize(-1);
	int stop = stop_indice(*indice);
	while (*indice <  stop)
	{
		if (ftokens[*indice] == "port")
		{
			handle_port(server, ports, indice);
			port = 1;//port is set
		}
		else if (ftokens[*indice] == "server_name")
		{
			server.setServerName(next_str_arg(indice));
			check_server_name_dup(server.getServerName());
			srv_name++;
		}
		else if (ftokens[*indice] == "client_max_body_size")
		{
			server.setBodySize(max_body_size(indice));
			body_size++;
		}
		else if (ftokens[*indice] == "hostname")
		{
			server.setHostName(next_str_arg(indice));
			host_name++;
		}
		else if (ftokens[*indice] == "error_page")
			handle_error_page(server, indice);
		else if (ftokens[*indice] == "location")
		{
			handle_locations(indice);
			handle_inside_locations(server, indice);
		}
		else if (!is_token(indice))
			error_call(ftokens[*indice] + " not refering to any directive!");
		(*indice)++;
	}
	if (!srv_name)
		error_call("Server_name not set for server ");
	if (port)
		server.setPorts(ports);
	else if ((server.getServerId() < 2 && !port))
		error_call("port not set in the first server!");
	else if (server.getServerId() > 1 && !port)
	{
		ports = this->directs[0].getPorts();
		server.setPorts(ports);
	}
	if(!srv_name || srv_name > 1)
		error_call("server_name must be set and not duplicated!");
	if (body_size > 1)
		error_call("client_max_body_size must not be duplicated!");
	if (host_name > 1)//!host_name ||
		error_call("hostname must be set and not duplicated!");
	server.server_locations = sort_location(server.getLocations());
	this->directs.push_back(server);
}

void Config::check_dup_location(std::vector<Location> locat)
{
	std::string str;
	if (locat.empty())
		return;
	for (size_t i = 0; i < locat.size() - 1; i++)
	{
		str = locat[i].getName();
		for (size_t j = i + 1; j < locat.size(); j++)
		{
			if (str == locat[j].getName())
				error_call("Location must not be duplicated!");
		}
	}
}

std::vector<Location> Config::sort_location(std::vector<Location> locat)
{
	Location tmp;
	int rootLocation = 0;
	check_dup_location(locat);
	if (locat.empty())
	{
		error_call("root location must be set!");
		return locat;
	}
	else if (locat.size() != 1)
	{
		for (size_t i = 0; i < locat.size() - 1; i++)
		{
			if (locat[i].getName() == "/")
			{
				tmp = locat[locat.size() - 1];
				locat[locat.size() - 1] = locat[i];
				locat[i] = tmp;
				rootLocation = 1;
			}
			for (size_t j = i + 1; j < locat.size(); j++)
			{
				if (locat[j].getName() == "/")
					rootLocation = 1;
				if (locat[i].getSlash() < locat[j].getSlash())
				{
					tmp = locat[i];
					locat[i] = locat[j];
					locat[j] = tmp;
				}
			}
		}
		if(!rootLocation)
			error_call("root location must be set!");
	}
	else
	{
		if (locat[0].getName() != "/")
			error_call("root location must be set!");
	}
	return locat;
}


bool Config::is_token(int *indice)
{
	if (ftokens[*indice].compare("{")
			&& ftokens[*indice].compare("}")
			&& ftokens[*indice].compare(";"))
				return false;
	return true;
}

void Config::fill_directive()
{
	int i;
	i = 0;
	if (ftokens[0].compare("server"))
		error_call("server missing in te begining");
	while (i < (int)ftokens.size())
	{
		if (ftokens[i] == "server")
			handle_servers(&i);
		i++;
	}
}

const std::string Config::get_page_error(std::string serv_name, int status)
{
	for (size_t i = 0; i < directs.size(); i++)
	{
		if (directs[i].getServerName() == serv_name)
			return (directs[i].getErrorPage(status));
	}
	return ("");
}

void Config::summarize()
{
	tokenizer("{");
	tokenizer("}");
	tokenizer(";");
	remove_spces();
	handle_brackets();
	fill_directive();
	// print_file2(*this);
}


Config::~Config()
{
	this->ftokens.clear();
}

// int main(int argc, char **argv)
// {
// 	if (argc != 2)
// 	{
// 		std::cerr << "Usage: ./config <config_file>" << std::endl;
// 		return (1);
// 	}
// 	Config conf(argv[1]);
// 	conf.summarize();
// 	return (0);
// }
