#include <iostream>
#include <string>
#include <map>
#include <fstream>
#include <vector>
#include <sstream>
#include <cctype>
#include <iterator>

#ifndef LOCA_HPP
#define LOCA_HPP

class Location
{
	private:
		std::string name;
		std::string path;
		std::string index;
		std::string root;
		std::string cgi_exe;
		std::vector<std::string> methods;
		std::string	return_str;
		bool	return_int;
		int		autoindex;
	public:
		Location();
		~Location();
		const std::string& getName() const;
		void setName(const std::string& name);
		const std::string& getIndex() const;
		void setIndex(const std::string& index);
		const std::string& getRoot() const;
		void setRoot(const std::string& root);
		const std::string& getCgiExe() const;
		void setCgiExe(const std::string& cgiExe);
		const std::vector<std::string>& getMethods() const;
		void setMethods(const std::vector<std::string>& methods);
		const std::string& getReturnString() const;
		void setReturnString(const std::string& returnStr);
		int getReturnInt() const;
		void setReturnInt(int returnInt);
		int getAutoindex() const;
		void setAutoindex(int autoindex);
};

#endif