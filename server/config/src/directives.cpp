#include "../include/directives.hpp"

Directives::Directives()
{
	body_size = 0;
}

Directives::~Directives()	{}

std::vector<Location> Directives::getLocations()
{
	return server_locations;
}


void Directives::setErrorPage(int key, std::string value)
{
	this->error_page.insert(std::make_pair(key, value));
}

const std::string Directives::getErrorPage(int status) const
{
	std::map<int, std::string>::const_iterator it;
	it = this->error_page.find(status);
	if (it != this->error_page.end())
		return (this->error_page.at(status));
	return ("");
}

int Directives::getServerId() const {
	return server_id;
}

void Directives::setServerId(int serverId) {
    server_id = serverId;
}

const std::string& Directives::getUploadPath() const {
	return upload_path;
}

void Directives::setUploadPath(const std::string& upload)
{
	if (upload[0] != '/' || upload[upload.length() - 1] != '/')
    {
		std::cerr << "\033[0;31m" << "Error: Invalid upload path" << "\033[0m" << std::endl;
		std::cerr << "\033[0;31m" << "upload path must begin and end with '/'" << "\033[0m" << std::endl;
		exit(1);
	}
	// else if (access(this. upload.c_str(), F_OK) == -1)
	// {
	// 	std::cerr << "\033[0;31m" << "Error: Invalid upload path" << "\033[0m" << std::endl;
	// 	std::cerr << "\033[0;31m" << "'"<< upload << "' does not exist" << "\033[0m" << std::endl;
	// 	exit(1);
	// }
    this->upload_path = upload;
}

const std::string& Directives::getServerName() const {
	return server_name;
}

void Directives::setServerName(const std::string& serverName) {
    server_name = serverName;
}

const std::vector<std::string>& Directives::getDefMethod() const {
	return def_method;
}

void Directives::setDefMethod(const std::vector<std::string>& method) {
    def_method = method;
}

const std::string& Directives::getHostName() const {
	return host_name;
}

void Directives::setHostName(const std::string& hostName) {
    host_name = hostName;
}

unsigned long long  Directives::getBodySize() const {
	return body_size;
}

void Directives::setBodySize(unsigned long long bodySize) {
    body_size = bodySize;
}

std::vector<int> Directives::getPorts()const
{
	return port;
}

void Directives::setPorts(std::vector<int>& ports)
{
	this->port = ports;
}
