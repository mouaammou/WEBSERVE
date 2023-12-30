#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <iostream>
class Server {
    private:
        int serverSocket;

    public:
        void startServer(int port) {
            // Create socket
            serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket == -1) {
                std::cerr << "Failed to create socket" << std::endl;
                return;
            }

            // Set socket options
            int opt = 1;
            if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)) == -1) {
                std::cerr << "Failed to set socket options" << std::endl;
                return;
            }

            // Set server address and port
            sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            serverAddress.sin_port = htons(port);

            // Bind socket to server address
            if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
                std::cerr << "Failed to bind socket" << std::endl;
                return;
            }

            // Start listening for connections
            if (listen(serverSocket, SOMAXCONN) == -1) {
                std::cerr << "Failed to listen for connections" << std::endl;
                return;
            }

            // Create pollfd structure for server socket
            std::vector<pollfd> pollFds(1);
            pollFds[0].fd = serverSocket;
            pollFds[0].events = POLLIN;

            // Start accepting connections
            while (true) {
                // Wait for events
                if (poll(pollFds.data(), pollFds.size(), -1) == -1) {
                    std::cerr << "Failed to poll for events" << std::endl;
                    return;
                }

                // Check for events on server socket
                if (pollFds[0].revents & POLLIN) {
                    // Accept incoming connection
                    int clientSocket = accept(serverSocket, nullptr, nullptr);
                    if (clientSocket == -1) {
                        std::cerr << "Failed to accept connection" << std::endl;
                        continue;
                    }

                    // Handle client connection
                    // TODO: Implement your logic here
                }
            }

            // Close server socket
            close(serverSocket);
        }
};
