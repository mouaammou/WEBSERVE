#include "directives.hpp"

Directives::Directives()
{
	//initialization:
	body_size = 0;
}

Directives::~Directives()
{
}
// hahowa li kaynin fihom les directives li kayna fihom les locations made by med
std::vector<Location> Directives::getLocations() const
{
	return server_locations;
}


void Directives::setErrorPage(int key, std::string value)
{
	this->error_page.insert(std::make_pair(key, value));
}

const std::string Directives::getErrorPage(int status) const
{
	if (this->error_page.find(status) != this->error_page.end())
		return (this->error_page.at(status));
	return ("");
}

int Directives::getServerId() const {
	return server_id;
}

void Directives::setServerId(int serverId) {
    server_id = serverId;
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

int Directives::getBodySize() const {
	return body_size;
}

void Directives::setBodySize(int bodySize) {
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