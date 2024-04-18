cmake_minimum_required(VERSION 3.0)
project(epoll)

set(CMAKE_CXX_STANDARD 11)

# Server executable
add_executable(server server.cpp)

add_executable(client client.cpp)

#add_executable(select_client select_client.cpp)
# Client executable
#add_executable(client client.cpp)
