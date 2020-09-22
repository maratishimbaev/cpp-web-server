#ifndef CPP_WEB_SERVER_CONFIG_H
#define CPP_WEB_SERVER_CONFIG_H

#include <string>
#include <fstream>
#include <iostream>

struct Config {
    unsigned int threadLimit;
    std::string documentRoot;
};

Config ParseConfig() {
    Config config{0, "../files"};

    std::ifstream file("../httpd.conf");
    std::string line;
    int pos;

    if (file) {
        std::getline(file, line);
        pos = line.find(' ');
        std::string value = line.substr(pos + 1, line.length());
        try {
            config.threadLimit = std::stoi(value);
        }
        catch (...) {
            std::cerr << "Thread limit is not a number\n";
        }

        std::getline(file, line);
        pos = line.find(' ');
        config.documentRoot = line.substr(pos + 1, line.length());
    } else {
        std::cerr << "Can't read config file\n";
    }

    return config;
}

#endif //CPP_WEB_SERVER_CONFIG_H
