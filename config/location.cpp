#include "location.hpp"

Location::Location() {
    // Constructor implementation
}

Location::~Location() {
    // Destructor implementation
}

const std::string& Location::getName() const {
	return name;
}

void Location::setName(const std::string& name) {
    this->name = name;
}

const std::string& Location::getIndex() const {
	return index;
}

void Location::setIndex(const std::string& index) {
    this->index = index;
}

const std::string& Location::getRoot() const {
	return root;
}

void Location::setRoot(const std::string& root) {
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

