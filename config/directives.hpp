#include "location.hpp"

#ifndef DIRECT_HPP
#define DIRECT_HPP

class Directives
{
		int server_id;
		std::string server_name;
		std::string host_name;
		int body_size;
		std::map<int, std::string> error_page;
	public:
		Directives();
		~Directives();
		std::vector<int> port;
		std::vector<Location> my_lct;
		int getServerId() const;
		void setServerId(int serverId);
		const std::string getErrorPage(int status) const;
		void setErrorPage(int key, std::string value);
		const std::string& getServerName() const;
		void setServerName(const std::string& serverName);
		const std::string& getHostName() const;
		void setHostName(const std::string& hostName);
		int getBodySize() const;
		void setBodySize(int bodySize);
};

#endif