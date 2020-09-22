#include "Handler.h"
#include <fstream>
#include <utility>
#include <unistd.h>
#include <iostream>
#include <ctime>
#include <string>

Handler::Handler(std::string documentRoot) : documentRoot(std::move(documentRoot)) {}

void Handler::Handle(int socket, std::string requestString) {
    Request request = ParseRequest(std::move(requestString));

    std::string status;
    std::vector<Header> headers = {
            Header{"Server", "cpp-web-server"},
            Header{"Date", GetNowDate()},
            Header{"Connection", "close"}
    };
    std::string body;

    if (request.method == "GET") {
        std::pair<std::string, int> pair = MakeBody(request.path);

        status = statuses[pair.second];

        if (pair.second == 200) {
            body = "\r\n\r\n" + pair.first;

            headers.push_back(Header{"Content-Type", GetContentType(request.path)});
            headers.push_back(Header{"Content-Length", std::to_string(pair.first.length() - 1)});
        }
    } else if (request.method == "HEAD") {
        std::pair<int, int> pair = GetFileSize(request.path);

        status = statuses[pair.second];

        if (pair.second == 200) {
            body = "\r\n\r\n";

            headers.push_back(Header{"Content-Type", GetContentType(request.path)});
            headers.push_back(Header{"Content-Length", std::to_string(pair.first)});
        }
    } else {
        status = statuses[405];
    }

    std::string message = "HTTP/1.1 " + status + MakeHeadersString(headers) + body;
    write(socket, message.c_str(), message.length());
    close(socket);
}

Request Handler::ParseRequest(std::string requestString) {
    Request request;

    int pos = requestString.find(' ');
    request.method = requestString.substr(0, pos);

    requestString = requestString.substr(pos + 1, requestString.length());
    pos = requestString.find(' ');
    request.path = DecodePath(requestString.substr(0, pos));

    // Delete query strings
    pos = request.path.find('?', 0);
    if (pos != std::string::npos) {
        request.path = request.path.substr(0, pos);
    }

    return request;
}

std::string Handler::DecodePath(std::string path) {
    std::string result;

    for (int i = 0; i < path.length(); i++) {
        if (path[i] == '%') {
            int ch;
            sscanf(path.substr(i + 1, 2).c_str(), "%x", &ch);
            result += static_cast<char>(ch);
            i += 2;
        } else if (path[i] == '+') {
            result += ' ';
        } else {
            result += path[i];
        }
    }

    return result;
}

std::string Handler::GetContentType(const std::string &path) {
    if (path.ends_with(".html")) { return "text/html"; }
    if (path.ends_with(".css")) { return "text/css"; }
    if (path.ends_with(".js")) { return "text/javascript"; }
    if (path.ends_with(".jpg") || path.ends_with(".jpeg")) { return "image/jpeg"; }
    if (path.ends_with(".png")) { return "image/png"; }
    if (path.ends_with(".gif")) { return "image/gif"; }
    if (path.ends_with(".swf")) { return "application/x-shockwave-flash"; }

    return "text/html";
}

std::string Handler::MakeHeadersString(const std::vector<Header> &headers) {
    std::string result;

    for (auto &header : headers) {
        result.append("\r\n" + header.name + ": " + header.value);
    }

    return result;
}

std::pair<std::string, int> Handler::MakeBody(std::string path) {
    std::string body;
    int status = 200;

    bool isDir = false;
    if (path.find('.', 0) == std::string::npos) {
        if (!path.ends_with("/")) {
            path += "/";
        }
        path += "index.html";
        isDir = true;
    }

    if (CheckPathEscape(path)) {
        return std::make_pair(body, 403);
    }

    std::ifstream file(documentRoot + path);
    if (file) {
        file.seekg(0, std::ios::end);
        size_t len = file.tellg();
        file.seekg(0);

        body = std::string(len + 1, '\0');
        file.read(&body[0], len);
    } else {
        if (isDir) {
            status = 403;
        } else {
            status = 404;
        }
    }

    return std::make_pair(body, status);
}

std::pair<int, int> Handler::GetFileSize(std::string path) {
    int size = 0;
    int status = 200;
    
    bool isDir = false;
    if (path.find('.', 0) == std::string::npos) {
        if (!path.ends_with("/")) {
            path += "/";
        }
        path += "index.html";
        isDir = true;
    }

    if (CheckPathEscape(path)) {
        return std::make_pair(size, 403);
    }
    
    std::ifstream file(documentRoot + path, std::ifstream::ate | std::ifstream::binary);
    if (file) {
        size = file.tellg();
    } else {
        if (isDir) {
            status = 403;
        } else {
            status = 404;
        }
    }
    
    return std::make_pair(size, status);
}

bool Handler::CheckPathEscape(std::string path) {
    path = path.substr(1, path.length());
    int dirLevel = 0;

    int pos;
    while ((pos = path.find('/', 0)) != std::string::npos) {
        std::string dir = path.substr(0, pos);

        if (dir == "..") {
            dirLevel--;
        } else {
            dirLevel++;
        }

        path = path.substr(pos + 1, path.length());
    }

    return (dirLevel < 0);
}

std::string Handler::GetNowDate() {
    char buf[100];
    time_t now = time(nullptr);
    tm tm = *gmtime(&now);
    strftime(buf, sizeof buf, "%a, %d %b %Y %H:%M^%S %Z", &tm);
    return std::string(buf);
}
