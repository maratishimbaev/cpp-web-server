cmake_minimum_required(VERSION 3.18)
project(cpp-web-server)

set(CMAKE_CXX_STANDARD 20)

set(HEADERS src/Request.h src/Response.h src/ThreadPool.h)

set(SOURCES src/Request.cpp src/Response.cpp src/ThreadPool.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(cpp-web-server ${HEADERS} ${SOURCES} src/main.cpp)