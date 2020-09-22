#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>
#include "ThreadPool.h"

int main(int argc, char* argv[]) {
    unsigned int port = 8000;
    unsigned int threadLimit = std::thread::hardware_concurrency();

    for (int i = 1; i < argc; i++) {
        if (i + 1 != argc) {
            if (strcmp(argv[i], "-p") == 0) {
                try {
                    port = std::stoi(std::string(argv[i + 1]));
                }
                catch (...) {
                    std::cout << "Wrong -p argument\n";
                }
                i++;
                continue;
            }
            if (strcmp(argv[i], "-t") == 0) {
                try {
                    threadLimit = std::stoi(std::string(argv[i + 1]));
                }
                catch (...) {
                    std::cout << "Wrong -t argument\n";
                }
            }
        }
    }

    int server;

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket is not created";
        return 0;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int addressLength = sizeof(address);

    if (bind(server, (struct sockaddr *)&address, addressLength) < 0) {
        std::cerr << "Socket is not bind";
        return 0;
    }

    if (listen(server, 10) < 0) {
        std::cerr << "Listen error";
        return 0;
    }

    auto threadPool = ThreadPool(threadLimit);

    std::cout << "Server started\n\n";

    while (true) {
        int socket;

        if ((socket = accept(server, (struct sockaddr *)&address, (socklen_t *)&addressLength)) < 0) {
            std::cerr << "Accept error";
            return 0;
        }

        int bufferSize = 10000;
        char buffer[bufferSize];
        read(socket, buffer, bufferSize);

        threadPool.AddToQueue(socket, std::string(buffer));
    }
}
