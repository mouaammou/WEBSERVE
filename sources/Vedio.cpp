#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>

const std::string VIDEO_FILE_PATH = "/goinfre/mouaammo/vedio.mp4";
const int PORT = 8080;

void sendVideo(int clientSocket) {
    // Read the video file
    std::ifstream videoFile(VIDEO_FILE_PATH, std::ios::binary | std::ios::ate);
    if (!videoFile.is_open()) {
        std::cerr << "Error opening video file\n";
        return ;
    }

	//this two line for the content length
    std::streampos fileSize = videoFile.tellg();
    videoFile.seekg(0, std::ios::beg);

	// Prepare HTTP response headers
	std::ostringstream response;
	response << "HTTP/1.1 200 OK\r\n";
	response << "Content-Type: video/mp4\r\n";
	response << "Content-Length: " << fileSize << "\r\n";
	response << "\r\n";

    // Send HTTP headers to the client
    std::string responseStr = response.str();
    send(clientSocket, responseStr.c_str(), responseStr.size(), 0);

    // Send video data to the client
    char buffer[1024];
    while (!videoFile.eof()) {
        videoFile.read(buffer, sizeof(buffer));
		if (videoFile.gcount() > 0)
			send(clientSocket, buffer, videoFile.gcount(), 0);
		else
			break;
    }

    // Close the video file
    videoFile.close();
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create a socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating socket\n";
        return 1;
    }

    // Set up the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket\n";
        close(serverSocket);
        return 1;
    }

    // Listen for incoming connections
	if (listen(serverSocket, 1) == -1) {
		std::cerr << "Error listening for connections\n";
		close(serverSocket);
		return 1;
	}

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddr), &clientAddrLen);
        if (clientSocket == -1) {
            std::cerr << "Error accepting connection\n";
            close(serverSocket);
            return 1;
        }

        std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << "\n";

        // Send video to the client
        sendVideo(clientSocket);

        // Close the client socket
        close(clientSocket);
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}
