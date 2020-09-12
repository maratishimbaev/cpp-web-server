#include "Request.h"

Request::Request(char *requestString) {
    std::string request(requestString);

    int pos;

    pos = request.find(' ');
    this->method = request.substr(0, pos);

    request = request.substr(pos + 1, request.length());
    pos = request.find(' ');
    this->path = request.substr(0, pos);
}

std::string Request::GetMethod() {
    return this->method;
}

std::string Request::GetPath() {
    return this->path;
}