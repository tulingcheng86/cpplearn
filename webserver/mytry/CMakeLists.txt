cmake_minimum_required(VERSION 3.0)
project(mytry)

set(CMAKE_CXX_STANDARD 11)

# Server executable
#add_executable(select_test select_test.cpp)

add_executable(select_server select_server.cpp)

add_executable(select_client select_client.cpp)
# Client executable
#add_executable(client client.cpp)
