// POST /upload HTTP/1.1
// Host: localhost
// Connection: keep-alive
// Content-Length: 461
// Cache-Control: max-age=0
// sec-ch-ua: "Not_A Brand";v="8", "Chromium";v="120", "Google Chrome";v="120"
// sec-ch-ua-mobile: ?0
// sec-ch-ua-platform: "macOS"
// Upgrade-Insecure-Requests: 1
// Origin: http://localhost
// Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryQfKkHui0vxFv0FvE
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.7
// Sec-Fetch-Site: same-origin
// Sec-Fetch-Mode: navigate
// Sec-Fetch-User: ?1
// Sec-Fetch-Dest: document
// Referer: http://localhost/upload/
// Accept-Encoding: gzip, deflate, br
// Accept-Language: en-US,en;q=0.9,ar-EG;q=0.8,ar;q=0.7


// ------WebKitFormBoundaryQfKkHui0vxFv0FvE
// Content-Disposition: form-data; name="file"; filename="txt"
// Content-Type: application/octet-stream

// read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:
// ------WebKitFormBoundaryQfKkHui0vxFv0FvE--

// POST /upload HTTP/1.1
// Host: example.com
// Content-Type: multipart/form-data; boundary=---------------------------1234567890123456789012345678

// -----------------------------1234567890123456789012345678
// Content-Disposition: form-data; name="text"

// some text
// -----------------------------1234567890123456789012345678
// Content-Disposition: form-data; name="file"; filename="example.txt"
// Content-Type: text/plain

// Contents of the file
// -----------------------------1234567890123456789012345678--


#include <iostream>
#include <fstream>
#include <string>
#include "../../includes/webserv.hpp"
#include "../../includes/Request.hpp"

class UploadFiles
{
	private:

	config		&args;
	std::string	boundary;
	std::string	end_boundary;
	std::string filename;
	bool		is_upload_request;

	public:

	UploadFiles(config &args) : args(args) {
		is_upload_request = false;
		if ( ! isMultipartAndValid())
			return;
		std::string body = args.request->getRequestBody();
		requestBody(body);
	}

	bool isUploadRequest( void )
	{
		return is_upload_request;
	}

	private:

	bool writeToFile( std::string &body )
	{
		std::cout << "-----------writing to file---------" << std::endl;
		std::ofstream *file = new std::ofstream();
		file->open(filename.c_str(), std::ios::out | std::ios::binary);
		if ( ! file->is_open())
		{
			std::cout << "failed to open file for uploaded file" << std::endl;
			//code 500
			return false;
		}
		file->write(body.c_str(), body.length());
		file->close();
		delete file;
		return true;
	}

	bool bodyContainsUploadedFile( std::string& headers)
	{
		std::cout << "headers->" << std::endl;
		size_t pos = headers.find("Content-Disposition:");
		if (pos == std::string::npos)
		{
			std::cout << "invalid request: Content-Disposition not found" << std::endl;
			return false;
		}
		headers.erase(0, pos + 20);
		pos = headers.find("\r\n");//always exists
		std::string line = headers.substr(0, pos);
		// std::cout << "line=" << line << std::endl;
		pos = line.find(";");//: form-data;
		size_t pos2 = line.find_first_not_of(" \t");
		std::string formdata = line.substr(pos2, pos - pos2);
		formdata.erase(formdata.find_last_not_of(" \t;") + 1);
		if (pos == std::string::npos || formdata != "form-data")
		{
			std::cout << "invalid request: form-data not found-->" << formdata << "<" << std::endl;
			return false;
		}
		line.erase(0, line.find_first_not_of(" \t", pos + 1));
		pos = line.find("filename=\"");
		if (pos == std::string::npos)
		{
			std::cout << "invalid request: filename not found" << std::endl;
			return false;
		}
		filename = line.substr(pos + 10);
		line.erase(pos);
		pos = line.find_last_not_of(" \t");
		if (pos != std::string::npos && line[pos] != ';')
		{
			std::cout << "invalid request: xxx filename not found" << std::endl;
			return false;
		}
		pos = filename.find("\"");
		if (pos == std::string::npos)
		{
			std::cout << "invalid request: vvv filename not found" << std::endl;
			return false;
		}
		filename.erase(pos);
		if (filename.empty())
		{
			std::cout << "invalid request: filename is empty" << std::endl;
			return false;
		}
		std::cout << "filename=" << filename << std::endl;
		is_upload_request = true;
		// std::cout << "filename --> "  << filename << std::endl;
		return true;
	}

	bool boundaryBody( std::string &body )
	{
		std::cout << "boundaryBody -->"  << std::endl;
		size_t pos = body.find("\r\n\r\n");
		if (pos == std::string::npos)
		{
			std::cout << "invalid request: end of headers not found" << std::endl;
			//code 4xx
			return false;
		}
		std::string header = body.substr(0, pos + 4);
		body.erase(0, pos + 4);
		if ( ! bodyContainsUploadedFile(header))
			return false;
		return writeToFile(body);
	}

	void requestBody( std::string &body )
	{
		std::cout << "requestBody" << std::endl;
		size_t pos = body.find(end_boundary);
		if (pos == std::string::npos)
		{
			std::cout << "invalid request: end boundary not found" << std::endl;
			//code 4xx
			return; //invalid request
		}
		body.erase(pos);
		pos = body.find(boundary);
		if (pos == std::string::npos)
		{
			std::cout << "invalid request: boundary not found" << std::endl;
			//code 4xx
			return; //invalid request
		}
		body.erase(0, pos + boundary.length());
		while (pos != std::string::npos)
		{
			pos = body.find(boundary);
			std::string buff = body.substr(0, pos);
			body.erase(0, pos + boundary.length());
			boundaryBody(buff);
		}
	}

	bool isMultipartAndValid( void )
	{
		std::map<std::string, std::string> mp = args.request->getRequestHeaders();
		std::map<std::string, std::string>::iterator it = mp.find("Content-Type:");
		std::string content = it->second;
		// std::string content = " multipart/form-data; boundary=----WebKitFormBoundaryQfKkHui0vxFv0FvE";
		size_t pos = content.find_first_not_of(" \t");
		content.erase(0, pos);
		pos = content.find(";");
		std::string multipart = content.substr(0, pos);
		// std::cout << "multipart=" << multipart << std::endl;
		content.erase(0, pos + 1);
		pos = multipart.find_last_not_of(" \t");
		multipart.erase(pos+1);
		if (multipart != "multipart/form-data")
		{
			std::cout << "not my request: suuccess" << multipart << std::endl;
			return false;//not my request: suuccess
		}
		pos = content.find_first_not_of(" \t");
		content.erase(0, pos);
		if (content.compare(0, 9, "boundary=") != 0)
		{
			std::cout << "invalid request: boundary not found" << std::endl;
			return false;//invalid request
		}
		content.erase(0, 9);
		if (content.empty())
		{
			std::cout << "invalid request: boundary is empty" << std::endl;
			return false;//invalid request
		}
		boundary = std::string("--") + content;
		end_boundary =  boundary + std::string("--\r\n");
		boundary.append("\r\n");
		// std::cout << "boundary=" << boundary << std::endl;
		// std::cout << "end_boundary=" << end_boundary << std::endl;
		return true;
	}
};

// std::string buff = "------WebKitFormBoundaryQfKkHui0vxFv0FvE\
// Content-Disposition: form-data; name=\"file\"; filename=\"txt\"\
// Content-Type: application/octet-stream\
// \
// read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:\
// ------WebKitFormBoundaryQfKkHui0vxFv0FvE--";
int main()
{
	UploadFiles upload;

	std::cout << "isUploadRequest=" << (upload.isUploadRequest()?"true":"false") << std::endl;
	return 0;
}
