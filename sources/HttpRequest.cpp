#include <iostream>
#include <sstream>
#include <map>
#include <string>

class HttpRequest {
	public:
		// Constructor to initialize the object with the raw HTTP request
		HttpRequest(const std::string& rawRequest) : rawRequest(rawRequest) {
			parseRequest();
		}

		// Getters to retrieve information from the parsed request
		const std::string& getMethod() const {
			return method;
		}

		const std::string& getPath() const {
			return path;
		}

		const std::map<std::string, std::string>& getHeaders() const {
			return headers;
		}

	private:
		// Raw HTTP request string
		std::string rawRequest;

		// Parsed components of the request
		std::string method;
		std::string path;
		std::map<std::string, std::string> headers;

		// Function to parse the raw HTTP request
		void parseRequest() {
			std::istringstream requestStream(rawRequest);
			std::string line;

			// Parse the request line
			if (std::getline(requestStream, line)) {
				parseRequestLine(line);
			}

			// Parse headers
			while (std::getline(requestStream, line) && !line.empty()) {
				parseHeader(line);
			}
		}

		// Function to parse the request line
		void parseRequestLine(const std::string& line) {
			std::istringstream lineStream(line);
			lineStream >> method >> path;
		}

		// Function to parse an individual header
		void parseHeader(const std::string& line) {
			std::size_t pos = line.find(':');
			if (pos != std::string::npos) {
				std::string key = line.substr(0, pos);
				std::string value = line.substr(pos + 1);
				// Trim leading and trailing whitespaces from the value
				value.erase(0, value.find_first_not_of(" \t"));
				value.erase(value.find_last_not_of(" \t") + 1);
				headers[key] = value;
			}
		}
};

int main() {
	// Example raw HTTP request
	std::string rawRequest =
		"GET /index.html HTTP/1.1\r\n"
		"Host: www.example.com\r\n"
		"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3\r\n"
		"\r\n";

	// Create an HttpRequest object and parse the raw request
	HttpRequest httpRequest(rawRequest);

	// Get and print information from the parsed request
	std::cout << "Method: " << httpRequest.getMethod() << std::endl;
	std::cout << "Path: " << httpRequest.getPath() << std::endl;

	const std::map<std::string, std::string>& headers = httpRequest.getHeaders();

	std::map<std::string, std::string>::const_iterator it = headers.begin();
	while (it != headers.end()) {
		std::cout << it->first << ": " << it->second << std::endl;
		++it;
	}

	return 0;
}
