#ifndef CPP_WEB_SERVER_RESPONSE_H
#define CPP_WEB_SERVER_RESPONSE_H

#include <string>

class Response {
public:
    explicit Response(std::string fileName);
    std::string GetMessage();

private:
    std::string fileName;
    std::string message;
};


#endif //CPP_WEB_SERVER_RESPONSE_H
