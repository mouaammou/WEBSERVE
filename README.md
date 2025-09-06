# Webserv

Webserv is a fully functional **HTTP/1.1 web server** implemented in **C++** as part of the 42 Network curriculum. It is designed to handle multiple client requests efficiently while supporting static files, CGI scripts, and customizable configurations.

---

## Features

- **HTTP/1.1 Support**: Handles `GET`, `POST`, and `DELETE` requests according to the HTTP/1.1 standard.  
- **Static File Serving**: Serve HTML, CSS, JavaScript, images, and other static content.  
- **CGI Support**: Execute CGI scripts for dynamic content generation.  
- **Multiple Clients**: Supports concurrent connections using `poll()` for efficient I/O multiplexing.  
- **Customizable Configuration**: Configure server ports, root directories, index files, and error pages.  
- **Error Handling**: Properly handles standard HTTP errors like 404 (Not Found), 500 (Internal Server Error), etc.  
- **Request Parsing**: Fully parses HTTP requests including headers, query strings, and body content.  
- **Robust and Secure**: Validates requests and prevents common server-side issues.

---

## Installation

Clone the repository:

```bash
git clone https://github.com/mouaammou/WEBSERVE.git
cd WEBSERVE
