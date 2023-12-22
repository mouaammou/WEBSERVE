#include "location.hpp"

#ifndef DIRECT_HPP
#define DIRECT_HPP

class Directives
{
	private:
		std::string server_name;
		std::string host_name;
		std::vector<std::string> def_method;
		std::map<int, std::string> error_page;
		std::vector<int> port;
		int server_id;
		int body_size;
	public:
		Directives();
		~Directives();
		std::vector<Location> server_locations;
		std::vector<Location> getLocations() const;// made by med, ghadi trajja3 lik ga3 location li kaynin fldakhl dyal server.
		int getServerId() const;
		void setServerId(int serverId);
		const std::string getErrorPage(int status) const;
		void setErrorPage(int key, std::string value);
		const std::string& getServerName() const;
		void setServerName(const std::string& serverName);
		const std::vector<std::string>& getDefMethod() const;
		void setDefMethod(const std::vector<std::string>& serverName);
		const std::string& getHostName() const;
		void setHostName(const std::string& hostName);
		int getBodySize() const;
		std::vector<int> getPorts()const;
		void setPorts(std::vector<int>& ports);
		void setBodySize(int bodySize);
};
#endif