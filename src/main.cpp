#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

#define PORT 8001
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

    char *message = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 7\n\nMessage";

    std::cout << "Server started\n\n";

    while (true) {
        int socket;

        if ((socket = accept(server, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            std::cerr << "Accept error";
            return 0;
        }

        char buffer[MAX_BUFFER_SIZE];
        read(socket, buffer, MAX_BUFFER_SIZE);
        std::cout << buffer;

        write(socket, message, strlen(message));
        std::cout << "Message sent\n\n";

        close(socket);
    }
}
