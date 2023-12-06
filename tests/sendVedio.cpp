#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Function prototypes
void sendVideoChunk(int clientSocket, const char* data, size_t size);
std::string generateHttpResponse(const char* contentType, size_t contentLength);

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    std::cout << "Server started on port 8080" << std::endl;

    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);

        // Assuming "video.mp4" is your video file
        std::ifstream videoFile("video.mp4", std::ios::binary);

        if (videoFile.is_open()) {
            std::ostringstream videoBuffer;
            videoBuffer << videoFile.rdbuf();
            std::string videoData = videoBuffer.str();

            // Send HTTP response header
            std::string responseHeader = generateHttpResponse("video/mp4", videoData.size());
            send(clientSocket, responseHeader.c_str(), responseHeader.size(), 0);

            // Send video data in chunks
            size_t chunkSize = 1024;  // Adjust the chunk size based on your needs
            size_t totalSize = videoData.size();
            for (size_t i = 0; i < totalSize; i += chunkSize) {
                size_t remainingSize = totalSize - i;
                size_t currentChunkSize = (remainingSize < chunkSize) ? remainingSize : chunkSize;
                sendVideoChunk(clientSocket, videoData.c_str() + i, currentChunkSize);
            }
        }

        close(clientSocket);
    }

    return 0;
}

void sendVideoChunk(int clientSocket, const char* data, size_t size) {
    // Send chunk size in hexadecimal followed by the chunk data
    std::string chunkHeader = std::to_string(size) + "\r\n";
    send(clientSocket, chunkHeader.c_str(), chunkHeader.size(), 0);
    send(clientSocket, data, size, 0);
    send(clientSocket, "\r\n", 2, 0);
}

std::string generateHttpResponse(const char* contentType, size_t contentLength) {
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n";
    response << "Content-Type: " << contentType << "\r\n";
    response << "Transfer-Encoding: chunked\r\n";
    response << "\r\n";
    return response.str();
}
