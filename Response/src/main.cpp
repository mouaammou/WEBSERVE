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

bool uploadFiles(config &args) {
	std::map<std::string, std::string> mp = args.request->getRequestHeaders();
	std::map<std::string, std::string>::iterator it = mp.find("Content-Type:");
	size_t pos = it->second.find("boundary=");
	if (it == mp.end() || it->second.find("multipart/form-data") == std::string::npos
		|| pos == std::string::npos)
		return false;
	std::string boundary = std::string("--") + it->second.substr(pos + 9) + "\r\n";
	std::string end = boundary + "--\r\n";
	std::string body = args.request->getRequestBody();
	pos = body.find(end);
	if (pos == std::string::npos)
		return false;
	body.erase(pos);
	pos = body.find(boundary);
	while (pos != std::string::npos)
	{
		body.erase(0, pos + boundary.length() - 2);//keep '\r\n'
		size_t pos = body.find("\r\n\r\n", pos);
		if (pos == std::string::npos)
			return false;
		std::string header(body.substr(0, pos));
		pos = header.find("\r\nContent-Disposition:");
		if (pos == std::string::npos)
			return false;
		std::string cd(header.substr(pos + 2, header.find("\r\n", pos + 2) - pos - 2));
		std::cout << "Content-Disposition:" << cd << std::endl;
		pos = body.find(boundary);
	}
}


std::string buff = "------WebKitFormBoundaryQfKkHui0vxFv0FvE\
Content-Disposition: form-data; name=\"file\"; filename=\"txt\"\
Content-Type: application/octet-stream\
\
read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:read:\
------WebKitFormBoundaryQfKkHui0vxFv0FvE--";
int main()
{
	post(buff, buff.length());
	return 0;
}
