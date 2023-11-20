#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

#define MAX_CLIENTS 10

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create a server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8080);  // Port 8080

    // Bind the server socket
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("bind");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("listen");
        close(serverSocket);
        exit(EXIT_FAILURE);
    }

    // Set up an array of pollfd structures for the server socket and connected clients
    struct pollfd fds[this->pollFds.size()];  // Plus one for the server socket
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    for (int i = 1; i < this->pollFds.size(); ++i) {
        fds[i].fd = -1;  // Initialize to an invalid file descriptor
        fds[i].events = POLLIN;
    }

    while (1) {
        // Call poll to wait for events
        int result = poll(fds, this->pollFds.size(), -1);  // Block indefinitely

        if (result == -1) {
            perror("poll");
            break;
        }

        // Check for events on the server socket (new client connection)
        if (fds[0].revents & POLLIN) {
            clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);

            if (clientSocket == -1) {
                perror("accept");
            } else {
                // Add the new client socket to the pollfd array
                for (int i = 1; i < this->pollFds.size(); ++i) {
                    if (fds[i].fd == -1) {
                        fds[i].fd = clientSocket;
                        break;
                    }
                }
            }
        }

        // Check for events on connected client sockets
        for (int i = 1; i < this->pollFds.size(); ++i) {
            if (fds[i].fd != -1 && (fds[i].revents & POLLIN)) {
                // Handle data from the client
                // ...
                
                // For simplicity, this example closes the client socket after handling data
                close(fds[i].fd);
                fds[i].fd = -1;  // Mark the slot as available
            }
        }
    }

    // Close all client sockets
    for (int i = 1; i < this->pollFds.size(); ++i) {
        if (fds[i].fd != -1) {
            close(fds[i].fd);
        }
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
