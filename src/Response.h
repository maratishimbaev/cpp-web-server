#ifndef CPP_WEB_SERVER_RESPONSE_H
#define CPP_WEB_SERVER_RESPONSE_H

#include <string>

class Response {
public:
    explicit Response(std::string fileName);
    std::string GetMessage();
    std::string GetContentType();
    static std::string DecodeURL(std::string url);

private:
    std::string path;
};


#endif //CPP_WEB_SERVER_RESPONSE_H
