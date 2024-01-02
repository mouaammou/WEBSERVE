
#include <stdio.h>
#include <string.h>
#include <sys/_types/_size_t.h>
#include <sys/types.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <sys/fcntl.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <dirent.h>

#include <csignal>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>


bool DeleteFolderContents(const std::string& directoryPath)
{
	DIR* dir = opendir(directoryPath.c_str());
	if (dir == NULL)
		return false;
	struct dirent* entry;
	while ((entry = readdir(dir)) != NULL)
	{
		std::string fileName = entry->d_name;
		if (fileName != "." && fileName != "..")
		{
			std::string fullPath = directoryPath + "/" + fileName;
			struct stat fileStat;
			if (stat(fullPath.c_str(), &fileStat) == 0)
			{
				if (S_ISDIR(fileStat.st_mode))
				{
					if (!DeleteFolderContents(fullPath))
					{
						closedir(dir);
						return false;
					}
				}
				else
				{
					if (remove(fullPath.c_str()) != 0)
					{
						closedir(dir);
						return false;
					}
				}
			}
			else
			{
				closedir(dir);
				return false;
			}
		}
	}
	closedir(dir);
	if (remove(directoryPath.c_str()) != 0)
		std::cout << "not removed\n";
	return true;
}


int main()
{
	if(DeleteFolderContents("arb"))
	{
		std::cout << "removed\n";
	}
	else
		std::cout << "not deleted\n";
	return 0;
}