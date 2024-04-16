cmake_minimum_required(VERSION 3.0)
project(TCPExample)

set(CMAKE_CXX_STANDARD 11)

# Server executable
add_executable(server server.cpp)

# Client executable
add_executable(client client.cpp)
