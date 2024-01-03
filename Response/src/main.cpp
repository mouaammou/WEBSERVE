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

#include <iostream>
#include <fstream>
#include <string>



void post(std::string buffer, int bytesRead) {
    // Handle file upload
    std::string boundary = "boundary=";
    size_t pos = buffer.find(boundary);
    if (pos != std::string::npos) {
        pos += boundary.length();
        std::string fileBoundary = "--" + buffer.substr(pos, buffer.find("\n", pos) - pos);

        // Find the start of file data
        pos = buffer.find(fileBoundary) + fileBoundary.length() + 2;
        pos = buffer.find("\n\n", pos) + 4;

        // Save the file data to a new file
        // std::ofstream outputFile("uploaded_file.txt", std::ios::binary);
        // outputFile.write(buffer.c_str() + pos, bytesRead - pos);
        // outputFile.close();
        std::cout << buffer.substr(pos, bytesRead - pos) << std::endl;
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
