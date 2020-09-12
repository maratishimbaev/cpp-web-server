#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "Request.h"
#include "Response.h"

#define PORT 8000
#define MAX_BUFFER_SIZE 10000

int main() {
    int server;

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket is not created";
        return 0;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    int addrlen = sizeof(address);

    if (bind(server, (struct sockaddr *)&address, addrlen) < 0) {
        std::cerr << "Socket is not bind";
        return 0;
    }

    if (listen(server, 10) < 0) {
        std::cerr << "Listen error";
        return 0;
    }

    std::cout << "Server started\n\n";

    while (true) {
        int socket;

        if ((socket = accept(server, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            std::cerr << "Accept error";
            return 0;
        }

        char buffer[MAX_BUFFER_SIZE];
        read(socket, buffer, MAX_BUFFER_SIZE);
//        std::cout << "----- Request -----\n" << buffer;

        Request request(buffer);

        std::string message;
        if (request.GetMethod() == "GET" || request.GetMethod() == "HEAD") {
            std::string path = request.GetPath();

            std::cout << "path: " << path << "\n";
            int pos = path.find('?', 0);
            if (pos != std::string::npos) {
                path = path.substr(0, pos);
                std::cout << "new path: " << path << "\n";
            }

            // Check if file escape directory root
            std::string pathCopy = path.substr(1, path.length());
            int dirCount = 0, escapeCount = 0;
            while ((pos = pathCopy.find('/', 0)) != std::string::npos) {
                std::string dir = pathCopy.substr(0, pos);

                if (dir == "..") {
                    escapeCount++;
                } else {
                    dirCount++;
                }

                pathCopy = pathCopy.substr(pos + 1, pathCopy.length());
            }

            if (escapeCount > dirCount) {
                message = "HTTP/1.1 403 Forbidden\nServer: cpp-web-server";
            } else {
                Response response(path);

                bool withFile = (request.GetMethod() == "GET");
                message = response.GetMessage(withFile);
            }
        } else {
            message = "HTTP/1.1 405 Method Not Allowed\nServer: cpp-web-server";
        }

//        std::cout << "----- Response -----\n" << message << "\n\n";
        write(socket, message.c_str(), message.length());

        close(socket);
    }
}
