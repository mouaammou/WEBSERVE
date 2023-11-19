#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

class Server {
private:
    int serverSocket;
    int clientSocket;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;
    char buffer[1024];

public:
    Server(int port) {
        // Create a socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            std::cerr << "Failed to create socket" << std::endl;
            return;
        }

        // Set up server address
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);

        // Bind the socket to the server address
        if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
            std::cerr << "Failed to bind socket" << std::endl;
            return;
        }

        // Listen for incoming connections
        if (listen(serverSocket, 5) == -1) {
            std::cerr << "Failed to listen for connections" << std::endl;
            return;
        }

        std::cout << "Server listening on port " << port << std::endl;
    }

    void acceptConnections() {
        socklen_t clientAddressLength = sizeof(clientAddress);

        // Accept incoming connections
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            std::cerr << "Failed to accept connection" << std::endl;
            return;
        }

        std::cout << "Accepted connection from " << inet_ntoa(clientAddress.sin_addr) << std::endl;

        // Receive and handle requests
        while (true) {
            memset(buffer, 0, sizeof(buffer));
            int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
            if (bytesRead == -1) {
                std::cerr << "Failed to receive data" << std::endl;
                break;
            } else if (bytesRead == 0) {
                std::cout << "Connection closed by client" << std::endl;
                break;
            }

            std::string request(buffer);
            std::cout << "Received request: " << request << std::endl;

            // Process the request and generate a response
            std::string response = generateResponse(request);

            // Send the response back to the client
            if (send(clientSocket, response.c_str(), response.length(), 0) == -1) {
                std::cerr << "Failed to send response" << std::endl;
                break;
            }
        }

        // Close the client socket
        close(clientSocket);
    }

    std::string generateResponse(const std::string& request) {
        // TODO: Implement your logic to generate the response based on the request
        // For example, you can parse the request to extract the requested resource and return a corresponding response

        std::string response = "HTTP/1.1 200 OK\r\n"
                               "Content-Type: text/html\r\n"
                               "\r\n"
                               "<html><body><h1>Hello, World!</h1></body></html>";
        
    
    std::string bigResponse = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: text/html\r\n"
                              "\r\n"
                              "<html><body><h1>Hello, World!</h1></body></html>"
                              "<p>This is a big response with multiple lines.</p>"
                              "<p>It can contain any HTML content you want.</p>"
                              "<p>Feel free to add more lines to customize it.</p>";

};

int main() {
    int port = 8080;
    Server server(port);
    server.acceptConnections();

    return 0;
}
