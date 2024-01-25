#include "../include/location.hpp"

Location::Location() {
	numOfSlash = 0;
    setAutoindex(-1);
}

Location::Location(std::string str) {
	if (str[0] != '/')
	{
		std::cerr << "\033[0;31m" << "THE LOCATION NAME : " + str + " MUST BEGIN WITH /" << "\033[0m" << std::endl;
		exit(1);
	}
	numOfSlash = 0;
    setAutoindex(-1);
	this->setName(str);
	if (this->getName().find("/") != std::string::npos)
		this->count_slash();
	if (str.find("//") != std::string::npos)
	{
		std::cerr << "\033[0;31m" << "TWO SLASHES IN A ROW ARE NOT ALLOWED IN LOCATION NAME" << "\033[0m" << std::endl;
		exit(1);
	}
}

Location::~Location() {
    // Destructor implementation
}

void Location::count_slash()
{
	std::string str;

	str = this->getName();
	for (size_t i = 0; i < str.size(); i++)
	{
		if (str[i] == '/')
			this->numOfSlash++;
	}
}

int Location::get_return_status() const
{
	return (this->return_status);
}


void Location::set_return_status(int status)
{
	if (status < 300 || status > 307)
	{
		std::cerr << "\033[0;31m" << "REDIRECT STATUS MUST BE BETWEEN 300 AND 307" << "\033[0m" << std::endl;
		exit(1);
	}
	this->return_status = status;
}


int Location::getSlash() const
{
	return (this->numOfSlash);
}


const std::string& Location::getName() const {
	return name;
}

void Location::setName(const std::string& name) {
	if (name[name.length() - 1] != '/')
	{
		std::cerr << "\033[0;31m" << "THE LOCATION NAME : " + name + " MUST END WITH /" << "\033[0m" << std::endl;
		exit(1);
	}
    this->name = name;
}

const std::string& Location::getIndex() const {
	return index;
}

void Location::setIndex(const std::string& index) {
	if (index.find("/") != std::string::npos)
	{
		std::cerr << "\033[0;31m" << "INDEX FILE MUST NOT CONTAIN A /" << "\033[0m" << std::endl;
		exit(1);
	}
    this->index = index;
}

const std::string& Location::getUploadPath() const {
	return upload_path;
}

void Location::setUploadPath(const std::string& path) {
	if (path.empty())
		return ;
	if (path[0] != '/')
	{
		std::cerr << "upload file must begin with a /" << std::endl;
		exit (1);
	}
    this->upload_path = path;
}
const std::string& Location::getRoot() const {
	return root;
}

void Location::setRoot(const std::string& root) {
	if (root[root.length() - 1] == '/')
	{
		std::cerr << "\033[0;31m" << "THE ROOT PATH : " + root + " MUST NOT END WITH /" << "\033[0m" << std::endl;
		exit(1);
	}
    this->root = root;
}

const std::string& Location::getCgiExe() const {
	return cgi_exe;
}

void Location::setCgiExe(const std::string& cgiExe) {
    cgi_exe = cgiExe;
}

const std::vector<std::string>& Location::getMethods() const {
	return methods;
}

void Location::setMethods(const std::vector<std::string>& methods) {
    this->methods = methods;
}

const std::string& Location::getReturnString() const {
	return return_str;
}

void Location::setReturnString(const std::string& 	returnStr) {
	return_str = returnStr;
}

int Location::getReturnInt() const {
	return return_int;
}

void Location::setReturnInt(int 	returnInt) {
	return_int = returnInt;
}

int Location::getAutoindex() const {
	return autoindex;
}

void Location::setAutoindex(int autoindex) {
	this->autoindex = autoindex;
}



bool Location::method_is_supported(std::string method)
{
	for (size_t i = 0; i < this->methods.size(); i++)
	{
		if (methods[i] == method)
			return true;
	}
	return false;
}
