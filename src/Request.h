#ifndef CPP_WEB_SERVER_REQUEST_H
#define CPP_WEB_SERVER_REQUEST_H

#include <string>
#include <map>

class Request {
public:
    explicit Request(std::string requestString);
    std::string GetMethod();
    std::string GetPath();

private:
    std::string method;
    std::string path;
};


#endif //CPP_WEB_SERVER_REQUEST_H
