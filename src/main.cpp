#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include "ThreadPool.h"
#include "config.h"

int main(int argc, char* argv[]) {
    unsigned int port = 8000;

    for (int i = 1; i < argc; i++) {
        if (i + 1 != argc) {
            if (strcmp(argv[i], "-p") == 0) {
                try {
                    port = std::stoi(std::string(argv[i + 1]));
                }
                catch (...) {
                    std::cerr << "Wrong -p argument\n";
                }
                i++;
                continue;
            }
        }
    }

    Config config = ParseConfig();

    int server;

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket is not created\n";
        return 0;
    }

    sockaddr_in address{};

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int addressLength = sizeof(address);

    if (bind(server, (struct sockaddr *)&address, addressLength) < 0) {
        std::cerr << "Socket is not bind\n";
        return 0;
    }

    if (listen(server, 10) < 0) {
        std::cerr << "Listen error\n";
        return 0;
    }

    ThreadPool threadPool(config.threadLimit, config.documentRoot);

    std::cout << "Server started\n\n";

    while (true) {
        int socket;

        if ((socket = accept(server, (struct sockaddr *)&address, (socklen_t *)&addressLength)) < 0) {
            std::cerr << "Accept error\n";
            return 0;
        }

        int bufferSize = 10000;
        char buffer[bufferSize];
        read(socket, buffer, bufferSize);

        threadPool.AddToQueue(socket, std::string(buffer));
    }
}
