#ifndef CPP_WEB_SERVER_THREADPOOL_H
#define CPP_WEB_SERVER_THREADPOOL_H

#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(unsigned int threadLimit, std::string documentRoot);
    ~ThreadPool();
    void AddToQueue(int socket, const std::string& requestString);

private:
    unsigned int threadNumber;
    std::string documentRoot;
    std::vector<std::thread> threads;
    std::queue<std::pair<int, std::string>> queue;
    std::mutex queueMutex;
    std::condition_variable_any queueConditionalVariable;
    bool done;
    void DoWork();
    void HandleRequest(int socket, const std::string& requestString);
};

#endif //CPP_WEB_SERVER_THREADPOOL_H
