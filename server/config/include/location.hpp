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
		std::string upload_path;
		std::string cgi_exe;
		std::vector<std::string> methods;
		std::string	return_str;
		int			return_status;
		bool	return_int;
		int		autoindex;
		int		numOfSlash;
	public:
		Location();
		Location(std::string str);
		~Location();
		const std::string& getName() const;
		void setName(const std::string& name);
		const std::string& getIndex() const;
		void setIndex(const std::string& index);
		const std::string& getRoot() const;
		void setRoot(const std::string& root);
		const std::string& getUploadPath() const;
		void setUploadPath(const std::string& root);
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

		bool method_is_supported(std::string method);
		void set_return_status(int status);
		int get_return_status() const;
		void count_slash();
		int getSlash() const;

};

#endif
