#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

const int PORT = 8080;
const int MAX_CONNECTIONS = 10;
const int MAX_EVENTS = 10;
const int BUFFER_SIZE = 1024;

class ChatServer {
public:
    ChatServer() : serverSocket(-1) {
        // Create socket
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocket == -1) {
            perror("Error creating socket");
            exit(EXIT_FAILURE);
        }

        // Bind to address and port
        sockaddr_in serverAddr;
		memset(&serverAddr, 0, sizeof(serverAddr));
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(PORT);

        if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
            perror("Error binding to address");
            exit(EXIT_FAILURE);
        }

        // Listen for connections
        if (listen(serverSocket, MAX_CONNECTIONS) == -1) {
            perror("Error listening for connections");
            exit(EXIT_FAILURE);
        }

        // Initialize poll structure
        fds[0].fd = serverSocket;
        fds[0].events = POLLIN;
        for (int i = 1; i < MAX_EVENTS; ++i) {
            fds[i].fd = -1;
        }
    }

    void run() {
        std::cout << "Chat server is running on port " << PORT << std::endl;

        while (true) {
            int numEvents = poll(fds, MAX_EVENTS, -1);
            if (numEvents == -1) {
                perror("Error in poll");
                exit(EXIT_FAILURE);
            }

            for (int i = 0; i < MAX_EVENTS; ++i) {
                if (fds[i].revents & POLLIN) {
                    if (fds[i].fd == serverSocket) {
                        acceptNewConnection();
                    } else {
                        handleClientData(i);
                    }
                }
            }
        }
    }

private:
    int serverSocket;
    struct pollfd fds[MAX_EVENTS];

    void acceptNewConnection() {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == -1) {
            perror("Error accepting connection");
            return;
        }

        // Add the new client socket to the poll set
        for (int i = 1; i < MAX_EVENTS; ++i) {
            if (fds[i].fd == -1) {
                fds[i].fd = clientSocket;
                fds[i].events = POLLIN;
                std::cout << "New client connected, socket: " << clientSocket << std::endl;
                break;
            }
        }
    }

    void handleClientData(int clientIndex) {
        char buffer[BUFFER_SIZE];
        int bytesRead = recv(fds[clientIndex].fd, buffer, sizeof(buffer), 0);
		buffer[bytesRead] = '\0';
        if (bytesRead <= 0) {
            // Connection closed or error
            std::cout << "Client disconnected, socket: " << fds[clientIndex].fd << std::endl;
            close(fds[clientIndex].fd);
            fds[clientIndex].fd = -1;
        } else {
            // Broadcast the received message to all clients
            broadcastMessage(clientIndex, buffer);
        }
    }

    void broadcastMessage(int senderIndex, const char* message) {
        std::cout << "Received message from client " << fds[senderIndex].fd << ": " << message << std::endl;

        for (int i = 1; i < MAX_EVENTS; ++i) {
            if (fds[i].fd != -1 && i != senderIndex) {
                send(fds[i].fd, message, strlen(message), 0);
            }
        }
    }
	
};

int main() {
    ChatServer server;
    server.run();

    return 0;
}


#include <iostream>
#include <sys/stat.h>
#include <cstring>

#define YOUR_PERMISSION_CHECK 0644 

bool isPathValid(const std::string& path) {
    struct stat info;

    // Check existence
    if (stat(path.c_str(), &info) != 0) {
        std::cerr << "Error accessing the path: " << strerror(errno) << std::endl;
        return false;
    }

    // Check if it's a directory
    if (S_ISDIR(info.st_mode)) {
        std::cout << "The path is a directory." << std::endl;
    } else if (S_ISREG(info.st_mode)) {
        std::cout << "The path is a regular file." << std::endl;
    } else {
        std::cout << "The path is neither a directory nor a regular file." << std::endl;
    }

    // Check permissions (replace 'YOUR_PERMISSION_CHECK' with the desired permission check)
    if ((info.st_mode & YOUR_PERMISSION_CHECK) != 0) {
        std::cout << "The program has the necessary permissions." << std::endl;
    } else {
        std::cout << "Insufficient permissions to access the path." << std::endl;
        return false;
    }

    // Additional checks based on your requirements

    return true;
}

int main() {
    std::string pathToCheck = "/your/path/to/check";

    if (isPathValid(pathToCheck)) {
        std::cout << "Path is valid." << std::endl;
    } else {
        std::cout << "Path is not valid." << std::endl;
    }

    return 0;
}
