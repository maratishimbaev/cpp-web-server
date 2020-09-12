#include "Response.h"
#include <fstream>
#include <utility>
#include <iostream>

#define STATUS_OK "200 OK"
#define STATUS_NOT_FOUND "404 Not Found"

Response::Response(std::string fileName) {
    this->fileName = std::move(fileName);
}

std::string Response::GetMessage() {
    std::string status = STATUS_OK;
    std::string contentType = GetContentType();
    std::string fileContent;

    std::ifstream file(fileName);
    if (file) {
        file.seekg(0, std::ios::end);
        size_t len = file.tellg();
        file.seekg(0);

        fileContent = std::string(len + 1, '\0');
        file.read(&fileContent[0], len);
    } else {
        status = STATUS_NOT_FOUND;
    }

    std::string message = "HTTP/1.1 " + status;
    if (!fileContent.empty()) {
        message.append("\nContent-Type: " + contentType + "\nContent-Length: " + std::to_string(fileContent.length()) + "\n\n" + fileContent);
    }

    return message;
}

std::string Response::GetContentType() {
    std::string name = this->fileName;

    if (name.ends_with(".html")) { return "text/html"; }
    if (name.ends_with(".css")) { return "text/css"; }
    if (name.ends_with(".js")) { return "text/javascript"; }
    if (name.ends_with(".jpg") || name.ends_with(".jpeg")) { return "image/jpeg"; }
    if (name.ends_with(".png")) { return "image/png"; }
    if (name.ends_with(".gif")) { return "image/gif"; }
    if (name.ends_with(".swf")) { return "application/x-shockwave-flash"; }

    return "text/plain";
}
