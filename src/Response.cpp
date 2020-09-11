#include "Response.h"
#include <fstream>
#include <utility>

Response::Response(std::string fileName) {
    this->fileName = std::move(fileName);

    this->message = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: ";
}

std::string Response::GetMessage() {
    std::ifstream file(fileName);
    if (file) {
        file.seekg(0, std::ios::end);
        size_t len = file.tellg();
        file.seekg(0);

        std::string fileContent(len + 1, '\0');
        file.read(&fileContent[0], len);

        message += std::to_string(fileContent.length());
        message += "\n\n";
        message += fileContent;
    }

    return message;
}
