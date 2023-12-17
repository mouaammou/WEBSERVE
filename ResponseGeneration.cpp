/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseGeneration.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: samjaabo <samjaabo@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/17 19:15:36 by samjaabo          #+#    #+#             */
/*   Updated: 2023/12/18 00:22:36 by samjaabo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>


class AutoIndex
{
	private:

	std::ostringstream		fin;
	std::string				root;//physical path
	std::string				uri;//uri path
	bool					error;

	void generate(void)
	{
		std::ostringstream		oss;

		oss << "<!DOCTYPE html>\n";
		oss << "<html>\n";
		oss << "\t<head>\n";
		oss << "\t\t<title>Index of " << uri << "</title>\n";
		oss << "\t\t<style type=\"text/css\">\n";
		oss << 	"\t\t\ttd {\n";
		oss << 	"\t\t\t\tpadding: 0 25px 0 0;\n";
		oss << 	"\t\t\t}\n";
		oss << 	"\t\t</style>\n";
		oss << "\t</head>\n";
		oss << "<body>\n";
		oss << "\t<h1>Index of " << uri << "</h1>\n";
		oss << "\t<hr>\n";

		oss << "\t<div>\n";
		oss << "\t\t<a href=\"../\">[&#128193; parent directory] &#8617;</a>\n";
		oss << "\t</div>\n";
		oss << "\t<br>\n";

		oss << "\t<table>\n";
		oss << "\t\t<thead>\n";
		oss << "\t\t\t<tr>\n";
		oss << "\t\t\t\t<td>Name</td>\n";
		oss << "\t\t\t\t<td>Size</td>\n";
		oss << "\t\t\t\t<td>Date Modified</td>\n";
		oss << "\t\t\t</tr>\n";
		oss << "\t\t</thead>\n";
		oss << "\t\t<tbody>\n";

		files( oss );

		oss << "\t\t</tbody>\n";
		oss << "\t</table>\n";
		oss << "\t<hr>\n";
		oss << "</body>\n";
		oss << "</html>\n";

		fin << "HTTP/1.1 200 OK\r\n";
		fin << "Content-Type: text/html\r\n";
		fin << "Content-Length: " << oss.str().length() << "\r\n";
		fin << "\r\n";
		fin << oss.str();
	}
	
	void files( std::ostringstream &oss )
	{
		DIR *dp = opendir((root + uri).c_str());
		if ( ! dp)
		{
			error = true;
			return ;
		}
		dirent *entry;
		while ((entry = readdir(dp)))
		{
			if (entry->d_name[0] != '.')
			{
				std::string filepath = root + uri + entry->d_name;
				std::string uri_name(entry->d_name);
				uri_name += dirUriFilter(filepath);
				std::string name = nameFilter(filepath, entry->d_name);
				int64_t size = fileSize(filepath);
				std::string date = fileLastModificationDate(filepath);

				oss << "\t\t\t<tr>\n";
				oss << "\t\t\t\t<td>" << "<a href=\"" << uri_name << "\">" << name << "</a>" << "</td>\n";
				oss << "\t\t\t\t<td>";
				if (size != -1)
					oss << formatSize(size);
				oss << "</td>\n";
				oss << "\t\t\t\t<td>" << date << "</td>\n";
				oss << "\t\t\t</tr>\n";
			}
		}
		closedir(dp);
	}

	std::string formatSize(int64_t bytes)
	{
		std::string units[] = {"B", "KB", "MB", "GB", "TB"};
		int i = 0;
		double size = static_cast<double>(bytes);

		while (size >= 1024.0 )
		{
			size /= 1024.0;
			i++;
		}

		std::ostringstream oss;
		oss << size;
		std::string str = oss.str();
		if (str.find(".") != std::string::npos)
			str.resize(str.find(".") + 2);
		return str.append(" ").append(units[i]);
	}

	std::string dirUriFilter( std::string const filePath )
	{
		struct stat fileStat;
		if (stat(filePath.c_str(), &fileStat) == 0)
		{
			if (S_ISDIR(fileStat.st_mode))
				return "/";
		}
		return "";
	}
	
	std::string nameFilter( std::string const filePath, std::string const name )
	{
		struct stat fileStat;
		if (stat(filePath.c_str(), &fileStat) == 0)
		{
			if (S_ISDIR(fileStat.st_mode))
				return std::string("&#128193; ") + name;
			else if (S_ISREG(fileStat.st_mode))
				return std::string("&#128196; ") + name;
		}
		return name;
	}

	std::string fileLastModificationDate( std::string const filePath )
	{
		struct stat fileStat;
		// std::cerr << filePath << std::endl;
		if (stat(filePath.c_str(), &fileStat) == 0)
		{
			
			char date[256];
			std::strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S GMT", std::gmtime(&fileStat.st_mtime));
			return date;
		}
		return "";
	}

	int64_t fileSize( std::string const filePath )
	{
		struct stat fileStat;
		if (stat(filePath.c_str(), &fileStat) == 0)
		{
			if (S_ISREG(fileStat.st_mode))
				return static_cast<int64_t>(fileStat.st_size);
		}
		return -1;
	}

	public:

	AutoIndex( std::string const &root, std::string const &uri )
	{
		this->root = root;
		this->uri = uri;
		error = false;
		generate();	
	}
	
	bool fail( void ) const
	{
		return error;
	}

	std::string const getOutput( void ) const
	{
		return fin.str();
	}
};

class ResponseGeneration
{
	private:
	std::ostringstream		headers;
	int		ffd;

	bool openFile( std::string const &path )
	{
		ffd = open(path.c_str(), O_RDONLY);
		if (ffd == -1)
			return false;
		return true;
	}

	int64_t getFileSize( void )
	{
		struct stat stat_buf;
		fstat(ffd, &stat_buf);
		return stat_buf.st_size;
	}

	public:
};
