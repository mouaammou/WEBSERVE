/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moouaamm <moouaamm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 14:38:58 by moouaamm          #+#    #+#             */
/*   Updated: 2023/12/31 02:42:22 by moouaamm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

int Config::count = 0;

Config::Config(/* args */)
{
}

std::vector<Directives> Config::get_directives() const
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
		error_call("Config file is empthy");
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
void Config::print_file2(Config &conf)
{
	size_t j = 0;
	std::vector<int> ports;
	while (j < conf.directs.size())
	{
		std::cout << "Server : " << j << std::endl;
		std::cout << conf.directs[j].getServerId() << std::endl;
		ports = conf.directs[j].getPorts();
		for (size_t i = 0; i < ports.size() ; i++)
		{
			std::cout << "port " << i << " " << ports[i] << std::endl;
		}
		std::cout << "server name    "<< conf.directs[j].getServerName() << std::endl;
		std::cout << "Error pages:" << std::endl;
		std::cout << "400 " << conf.directs[j].getErrorPage(400)<< std::endl;
		std::cout << "403 " << conf.directs[j].getErrorPage(403)<< std::endl;
		std::cout << "201 " << conf.directs[j].getErrorPage(201)<< std::endl;
		std::cout << "500 " << conf.directs[j].getErrorPage(500)<< std::endl;
		std::cout << "504 " << conf.directs[j].getErrorPage(504)<< std::endl;
		std::cout << "body_size    "<< conf.directs[j].getBodySize() << std::endl;
		std::cout << "host_name    "<< conf.directs[j].getHostName() << std::endl;
		std::cout << "Locations for server "<< j << std::endl;
		for (size_t i = 0; i < conf.directs[j].server_locations.size(); i++)
		{
			std::cout<<"name " << conf.directs[j].server_locations[i].getName() << std::endl;
			std::cout<<"root " << conf.directs[j].server_locations[i].getRoot() << std::endl;
			std::cout<<"auto index " << conf.directs[j].server_locations[i].getAutoindex() << std::endl;
			std::cout<<"index " << conf.directs[j].server_locations[i].getIndex() << std::endl;
			if (conf.directs[j].server_locations[i].getReturnInt())
				std::cout<<"return " << conf.directs[j].server_locations[i].getReturnString() << std::endl;
			if (!conf.directs[j].server_locations[i].getCgiExe().empty())
			std::cout<<"exec " << conf.directs[j].server_locations[i].getCgiExe() << std::endl;
			std::cout << "Methodes ";
			std::vector<std::string> methods;
			methods = conf.directs[j].server_locations[i].getMethods();
			for (size_t k = 0; k < methods.size(); k++)
			{
				std::cout << methods[k] << "   ";
			}
			std::cout << std::endl;
		}
		j++;
	}
}

void Config::handle_brackets(void)
{
	std::string brackets;
	for (size_t i = 0; i < this->ftokens.size(); i++)
	{
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
	while ( i < str.size())
	{
		if (!isdigit(str[i]))
			return false;
		i++;
	}
	return true;
}

void Config::error_page(std::vector<std::string> vec, int indice)
{
	for (size_t i = 1; indice + i < vec.size(); i++)
	{
		if (!str_digit(vec[indice + i]))
		{
			std::cout << "Error :" << vec[indice + i - 1] << " " << vec[indice + i] << " Not digit" <<std::endl;
			exit(0);
		}
	}
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
	std::cerr << error << std::endl;
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


void Config::handle_default_method(Directives& server, int *indice)
{
	int tmp;
	std::vector<std::string> methods;
	tmp = (*indice);
	while (ftokens[tmp].compare(";"))
		tmp++;
	(*indice)++;
	if ((*indice) == tmp || tmp - (*indice) > 3)
		error_call("error default_method!");
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
	server.setDefMethod(methods);
}

void Config::handle_inside_locations(Directives& server, int *indice)
{
	Location locat(ftokens[*indice]);
	int root = 0;
	locat.setReturnInt(0);
	(*indice)++;
	while (ftokens[*indice].compare("}") && *indice < (int)ftokens.size())
	{
		if (ftokens[*indice] == "root")
		{
			locat.setRoot(next_str_arg(indice));
			root = 1;
		}
		else if (ftokens[*indice] == "return")
			handle_redirection(locat, indice);
		else if (ftokens[*indice] == "autoindex")
			locat.setAutoindex(handle_autodx(indice));
		else if (ftokens[*indice] == "exe")
			locat.setCgiExe(next_str_arg(indice));
		else if (ftokens[*indice] == "proxy_method")
			handle_proxy_methode(locat, indice);
		else if (ftokens[*indice] == "index")
			locat.setIndex(next_str_arg(indice));
		else if (!is_token(indice))
			error_call(ftokens[*indice] + " not refer to any arg inside location!");
		(*indice)++;
	}
	if (!locat.getReturnInt() && !root)
		error_call("root must be present in location section : " + locat.getName());
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
	if (!str_digit(ftokens[*indice]))
		error_call("port must be digit!");
	int port;
	port = atoi(ftokens[*indice].c_str());
	if (port < 0 || port > 65535)
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

int Config::max_body_size(int *indice)
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
	if (!str_digit(ftokens[*indice]))
		error_call("max body size must be digits!");
	long long max;;
	max = atoll(ftokens[*indice].c_str());
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

void Config::set_default_pages(Directives &server)
{
	std::string tmp;
	std::stringstream number;
	for (int i = 400; i < 417; i++)
	{
		number << i;
		tmp = server.getErrorPage(i);
		if (tmp.empty())
			server.setErrorPage(i, std::string("./www/error/") + number.str() + ".html");
		number.str("");
	}
	for (int i = 500; i < 505; i++)
	{
		number << i;
		tmp = server.getErrorPage(i);
		if (tmp.empty())
			server.setErrorPage(i, std::string("./www/error/") + number.str() + ".html");
		number.str("");
	}
}

void Config::handle_servers(int *indice)
{
	Directives server;
	std::vector<int> ports;
	int size;
	int port = 0;
	int srv_name = 0;
	size = this->ftokens.size();
	if (size == *indice + 1)
		error_call("server not set!");
	if (this->ftokens[*indice + 1] != "{")
		error_call("brackets { not open after server!");
	(*indice)++;
	this->count++;
	server.setServerId(this->count);
	while ((size_t)*indice < ftokens.size() && ftokens[*indice].compare("server"))
	{
		if (ftokens[*indice] == "port")
		{
			handle_port(server, ports, indice);
			port = 1;
		}
		else if (ftokens[*indice] == "server_name")
		{
			server.setServerName(next_str_arg(indice));
			check_server_name_dup(server.getServerName());
			srv_name = 1;
		}
		else if (ftokens[*indice] == "client_max_body_size")
			server.setBodySize(max_body_size(indice));
		else if (ftokens[*indice] == "hostname")
			server.setHostName(next_str_arg(indice));
		else if (ftokens[*indice] == "error_page")
			handle_error_page(server, indice);
		else if (ftokens[*indice] == "location")
		{
			handle_locations(indice);
			handle_inside_locations(server, indice);
		}
		else if (ftokens[*indice] == "default_method")
			handle_default_method(server, indice);
		else if (!is_token(indice))
			error_call(ftokens[*indice] + " not refering to any directive!");
		(*indice)++;
	}
	if (!srv_name)
		error_call("Server name not set for server ");
	if (port)
		server.setPorts(ports);
	else if (server.getServerId() < 2 && !port)
		error_call("port not set in the first server!");
	else if (server.getServerId() > 1 && !port)
	{
		ports = this->directs[0].getPorts();
		server.setPorts(ports);
	}
	server.server_locations = sort_location(server.getLocations());
	set_default_pages(server);
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
	check_dup_location(locat);
	if (locat.empty())
		return locat;
	for (size_t i = 0; i < locat.size() - 1; i++)
	{
		if (locat[i].getName() == "/")
		{
			tmp = locat[locat.size() - 1];
			locat[locat.size() - 1] = locat[i];
			locat[i] = tmp;
		}
		for (size_t j = i + 1; j < locat.size(); j++)
		{
			if (locat[i].getSlash() < locat[j].getSlash())
			{
				tmp = locat[i];
				locat[i] = locat[j];
				locat[j] = tmp;
			}
		}
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
}


Config::~Config()
{
	this->ftokens.clear();
}
// const server_data Config::search_uri(int serverId, std::string uri)
// {
// 	for (size_t i = 0; i < this->directs.size(); i++)
// 	{
// 		if (directs[i].getServerId() == serverId)
// 		{
// 			return directs[i].my_lct[i];
// 			directs[i].getBodySize();
// 		}
// 	}

// }

int main(int argc, char *argv[])
{
	if (argc == 2)
	{
		// directives dir;
		std::string file;
		file = argv[1];
		Config conf(file);
		conf.summarize();
		// conf.print_file2(conf);
		std::vector<Directives> directs;
		directs = conf.get_directives();
		size_t j = 0;
	std::vector<int> ports;
	while (j < directs.size())
	{
		std::cout << "Server : " << j << std::endl;
		std::cout << directs[j].getServerId() << std::endl;
		ports = directs[j].getPorts();
		for (size_t i = 0; i < ports.size() ; i++)
		{
			std::cout << "port " << i << " " << ports[i] << std::endl;
		}
		std::cout << "server name    "<< directs[j].getServerName() << std::endl;
		std::cout << "Error pages:" << std::endl;
		std::cout << "400 " << directs[j].getErrorPage(400)<< std::endl;
		std::cout << "101 " << directs[j].getErrorPage(101)<< std::endl;
		std::cout << "403 " << directs[j].getErrorPage(403)<< std::endl;
		std::cout << "404 " << directs[j].getErrorPage(404)<< std::endl;
		std::cout << "413 " << directs[j].getErrorPage(413)<< std::endl;
		std::cout << "202 " << directs[j].getErrorPage(202)<< std::endl;
		std::cout << "205 " << directs[j].getErrorPage(205)<< std::endl;
		std::cout << "504 " << directs[j].getErrorPage(504)<< std::endl;
		std::cout << "502 " << directs[j].getErrorPage(502)<< std::endl;
		std::cout << "503 " << directs[j].getErrorPage(503)<< std::endl;
		std::cout << "body_size    "<< directs[j].getBodySize() << std::endl;
		std::cout << "host_name    "<< directs[j].getHostName() << std::endl;
		std::cout << "Locations for server "<< j << std::endl;
		for (size_t i = 0; i < directs[j].server_locations.size(); i++)
		{
			std::cout<<"Slashes " << directs[j].server_locations[i].getSlash() << std::endl;
			std::cout<<"name " << directs[j].server_locations[i].getName() << std::endl;
			std::cout<<"root " << directs[j].server_locations[i].getRoot() << std::endl;
			std::cout<<"auto index " << directs[j].server_locations[i].getAutoindex() << std::endl;
			std::cout<<"index " << directs[j].server_locations[i].getIndex() << std::endl;
			if (directs[j].server_locations[i].getReturnInt())
				std::cout<<"return " << directs[j].server_locations[i].getReturnString() << std::endl;
			if (!directs[j].server_locations[i].getCgiExe().empty())
			std::cout<<"exec " << directs[j].server_locations[i].getCgiExe() << std::endl;
			std::cout << "Methodes ";
			std::vector<std::string> methods;
			methods = directs[j].server_locations[i].getMethods();
			for (size_t k = 0; k < methods.size(); k++)
			{
				std::cout << methods[k] << "   ";
			}
			std::cout << std::endl;
		}
		j++;
	}
		return 0;
	}
	std::cout << "Error usage: <./webserv config_file>" << std::endl;
}
