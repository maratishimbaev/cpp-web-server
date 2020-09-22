#include "ThreadPool.h"
#include "Handler.h"
#include <utility>

ThreadPool::ThreadPool(unsigned int threadLimit, std::string documentRoot) : done(false), documentRoot(std::move(documentRoot)) {
    threadNumber = threadLimit;
    if (threadNumber == 0) {
        threadNumber = 1;
    }

    for (int i = 0; i < threadNumber; i++) {
        threads.emplace_back(&ThreadPool::DoWork, this);
    }
}

ThreadPool::~ThreadPool() {
    done = true;

    queueConditionalVariable.notify_all();
    for (auto &thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::AddToQueue(int socket, const std::string &requestString) {
    std::lock_guard<std::mutex> g(queueMutex);
    queue.push(std::pair<int, std::string>(socket, requestString));
    queueConditionalVariable.notify_one();
}

void ThreadPool::DoWork() {
    while (!done) {
        std::pair<int, std::string> request;

        {
            std::unique_lock<std::mutex> g(queueMutex);
            queueConditionalVariable.wait(g, [&]{
                return !queue.empty() || done;
            });

            request = queue.front();
            queue.pop();
        }

        HandleRequest(request.first, request.second);
    }
}

void ThreadPool::HandleRequest(int socket, const std::string &requestString) {
    Handler handler(documentRoot);
    handler.Handle(socket, requestString);
}
