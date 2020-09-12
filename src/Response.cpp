#include "Response.h"
#include <fstream>
#include <utility>
#include <iostream>
#include <sstream>
#include <cstdio>

#define STATUS_OK "200 OK"
#define STATUS_NOT_FOUND "404 Not Found"
#define STATUS_FORBIDDEN "403 Forbidden"

#define DIR_ROOT "../files"

Response::Response(std::string fileName) {
    this->path = std::move(fileName);
}

std::string Response::GetMessage(bool withFile) {
    std::string status = STATUS_OK;
    std::string contentType = GetContentType();
    std::string fileContent;

    bool isDir = false;
    if (this->path.ends_with("/")) {
        path += "index.html";
        isDir = true;
    }

    path = DecodeURL(path);

    std::ifstream file(DIR_ROOT + path);
    if (file) {
        file.seekg(0, std::ios::end);
        size_t len = file.tellg();
        file.seekg(0);

        fileContent = std::string(len + 1, '\0');
        file.read(&fileContent[0], len);
    } else {
        if (!isDir) {
            status = STATUS_NOT_FOUND;
        } else {
            status = STATUS_FORBIDDEN;
        }
    }

    std::string message = "HTTP/1.1 " + status + "\r\nServer: cpp-web-server";
    if (!fileContent.empty()) {
        message.append("\r\nContent-Type: " + contentType +
            "\r\nContent-Length: " + std::to_string(fileContent.length() - 1) + "\r\n\r\n");
        if (withFile) {
            message.append(fileContent);
        }
    }

    return message;
}

std::string Response::GetContentType() {
    std::string name = this->path;

    if (name.ends_with(".html")) { return "text/html"; }
    if (name.ends_with(".css")) { return "text/css"; }
    if (name.ends_with(".js")) { return "text/javascript"; }
    if (name.ends_with(".jpg") || name.ends_with(".jpeg")) { return "image/jpeg"; }
    if (name.ends_with(".png")) { return "image/png"; }
    if (name.ends_with(".gif")) { return "image/gif"; }
    if (name.ends_with(".swf")) { return "application/x-shockwave-flash"; }

    return "text/plain";
}

std::string Response::DecodeURL(std::string url) {
    std::string result;

    for (int i = 0; i < url.length(); i++) {
        if (url[i] == '%') {
            int ch;
            sscanf(url.substr(i + 1, 2).c_str(), "%x", &ch);
            result += static_cast<char>(ch);
            i += 2;
        } else if (url[i] == '+') {
            result += ' ';
        } else {
            result += url[i];
        }
    }

    return result;
}
