#include "ThreadPool.h"
#include "Request.h"
#include "Response.h"
#include <iostream>
#include <utility>
#include <unistd.h>

ThreadPool::ThreadPool() : done(false) {
    threadNumber = std::thread::hardware_concurrency();
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

void ThreadPool::HandleRequest(int socket, std::string requestString) {
    Request request(std::move(requestString));

    std::string message;
    if (request.GetMethod() == "GET" || request.GetMethod() == "HEAD") {
        std::string path = request.GetPath();

        int pos = path.find('?', 0);
        if (pos != std::string::npos) {
            path = path.substr(0, pos);
        }

        // Check if file escape directory root
        std::string pathCopy = path.substr(1, path.length());
        int dirCount = 0, escapeCount = 0;
        while ((pos = pathCopy.find('/', 0)) != std::string::npos) {
            std::string dir = pathCopy.substr(0, pos);

            if (dir == "..") {
                escapeCount++;
            } else {
                dirCount++;
            }

            pathCopy = pathCopy.substr(pos + 1, pathCopy.length());
        }

        if (escapeCount > dirCount) {
            message = "HTTP/1.1 403 Forbidden\nServer: cpp-web-server";
        } else {
            Response response(path);

            bool withFile = (request.GetMethod() == "GET");
            message = response.GetMessage(withFile);
        }
    } else {
        message = "HTTP/1.1 405 Method Not Allowed\nServer: cpp-web-server";
    }

    std::cout << "----- Response -----\n" << message << "\n\n";
    write(socket, message.c_str(), message.length());

    close(socket);
}
