#include <iostream>
#include <string>
#include <vector>

class Request {
	private:
		std::string method;
		std::string path;
		std::string version;
		std::vector<std::string> headers;
		std::string body;

	public:
		Request(const std::string& requestString) {
			parseRequest(requestString);
		}

		void parseRequest(const std::string& requestString) {
			// Find the end of the request line
			size_t requestLineEnd = requestString.find("\r\n");
			if (requestLineEnd == std::string::npos) {
				std::cerr << "Invalid request: missing request line" << std::endl;
				return;
			}

			// Parse the request line
			std::string requestLine = requestString.substr(0, requestLineEnd);
			size_t methodEnd = requestLine.find(' ');
			if (methodEnd == std::string::npos) {
				std::cerr << "Invalid request: missing method" << std::endl;
				return;
			}
			method = requestLine.substr(0, methodEnd);

			size_t pathEnd = requestLine.find(' ', methodEnd + 1);
			if (pathEnd == std::string::npos) {
				std::cerr << "Invalid request: missing path" << std::endl;
				return;
			}
			path = requestLine.substr(methodEnd + 1, pathEnd - methodEnd - 1);

			version = requestLine.substr(pathEnd + 1);

			// Find the start of the headers
			size_t headersStart = requestLineEnd + 2;

			// Find the end of the headers
			size_t headersEnd = requestString.find("\r\n\r\n", headersStart);
			if (headersEnd == std::string::npos) {
				std::cerr << "Invalid request: missing headers" << std::endl;
				return;
			}

			// Parse the headers
			std::string headersString = requestString.substr(headersStart, headersEnd - headersStart);
			size_t headerStart = 0;
			size_t headerEnd = headersString.find("\r\n");
			while (headerEnd != std::string::npos) {
				headers.push_back(headersString.substr(headerStart, headerEnd - headerStart));
				headerStart = headerEnd + 2;
				headerEnd = headersString.find("\r\n", headerStart);
			}

			// Find the start of the body
			size_t bodyStart = headersEnd + 4;

			// Parse the body
			body = requestString.substr(bodyStart);
		}

		std::string getMethod() const {
			return method;
		}

		std::string getPath() const {
			return path;
		}

		std::string getVersion() const {
			return version;
		}

		std::vector<std::string> getHeaders() const {
			return headers;
		}

		std::string getBody() const {
			return body;
		}
};

int main() {
	std::string requestString = "GET /index.html HTTP/1.1\r\n"
								"Host: example.com\r\n"
								"User-Agent: Mozilla/5.0\r\n"
								"Accept-Language: en-US,en;q=0.5\r\n"
								"\r\n"
								"This is the request body.";

	Request request(requestString);

	std::cout << "Method: " << request.getMethod() << std::endl;
	std::cout << "Path: " << request.getPath() << std::endl;
	std::cout << "Version: " << request.getVersion() << std::endl;

	std::cout << "Headers:" << std::endl;
	std::vector<std::string> headers = request.getHeaders();
	for (size_t i = 0; i < headers.size(); i++) {
		std::cout << headers[i] << std::endl;
	}

	std::cout << "Body: " << request.getBody() << std::endl;

	return 0;
}
