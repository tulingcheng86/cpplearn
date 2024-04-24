cmake_minimum_required(VERSION 3.0)
project(thread)

set(CMAKE_CXX_STANDARD 11)

# Server executable
add_executable(test test.cpp)
add_executable(client client.cpp)
add_executable(server server.cpp)

# 链接pthread库
target_link_libraries(client PRIVATE pthread)
target_link_libraries(server PRIVATE pthread)

# 设置编译选项以消除警告
target_compile_options(client PRIVATE -Wall -Wextra -pedantic)
target_compile_options(server PRIVATE -Wall -Wextra -pedantic)
