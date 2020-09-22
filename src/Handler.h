#ifndef CPP_WEB_SERVER_HANDLER_H
#define CPP_WEB_SERVER_HANDLER_H

#include <string>
#include <vector>
#include <map>

struct Request {
    std::string method;
    std::string path;
};

struct Header {
    std::string name;
    std::string value;
};

class Handler {
public:
    explicit Handler(std::string documentRoot);
    void Handle(int socket, std::string requestString);

private:
    static Request ParseRequest(std::string requestString);
    static std::string DecodePath(std::string path);
    static std::string GetContentType(const std::string& path);
    static std::string MakeHeadersString(const std::vector<Header>& headers);
    std::pair<std::string, int> MakeBody(std::string path);
    std::pair<int, int> GetFileSize(std::string path);
    static bool CheckPathEscape(std::string path);
    static std::string GetNowDate();
    std::string documentRoot;
    std::map<int, std::string> statuses = {
            {200, "200 OK"},
            {404, "404 Not Found"},
            {403, "403 Forbidden"},
            {405, "405 Method Not Allowed"}
    };
};

#endif //CPP_WEB_SERVER_HANDLER_H
