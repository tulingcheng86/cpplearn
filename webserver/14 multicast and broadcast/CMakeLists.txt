cmake_minimum_required(VERSION 3.0)
project(select)

set(CMAKE_CXX_STANDARD 11)

# Server executable
add_executable(sender sender.cpp)

add_executable(receiver receiver.cpp)

add_executable(2_sender 2_sender.cpp)

add_executable(2_receiver 2_receiver.cpp)
#add_executable(select_client select_client.cpp)
# Client executable
#add_executable(client client.cpp)
