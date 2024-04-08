#include <iostream>     // 标准输入输出流
#include <sstream>      // 字符串流
#include <string>       // 字符串处理
#include <sys/socket.h> // socket 相关函数
#include <netinet/in.h> // IPv4 地址结构
#include <unistd.h>     // Unix 系统调用
#include <cstring>      // 字符串处理函数

// 处理客户端请求的函数
void serve(int client_socket) {
    // HTTP 响应内容
    const char* http_response =
        "HTTP/1.1 200 OK\r\n"       // HTTP 响应头，200 表示成功
        "Content-Type: text/html\r\n" // 内容类型为 HTML
        "\r\n"                       // 响应头和响应体之间的空行
        "<html><body><h1>Hello, World!</h1></body></html>\r\n"; // HTML 页面内容

    // 将 HTTP 响应发送给客户端
    write(client_socket, http_response, strlen(http_response));
    // 关闭与客户端的连接
    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // 创建 socket 文件描述符
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("In socket"); // 打印错误信息
        exit(EXIT_FAILURE); // 退出程序
    }

    // 设置地址结构
    address.sin_family = AF_INET;         // IPv4 地址族
    address.sin_addr.s_addr = INADDR_ANY; // 监听所有网络接口上的连接
    address.sin_port = htons(8080);        // 设置监听端口为 8080

    // 将 socket 与指定端口绑定
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0) {
        perror("In bind"); // 打印错误信息
        exit(EXIT_FAILURE); // 退出程序
    }
    // 开始监听连接请求，设置最大连接数为 10
    if (listen(server_fd, 10) < 0) {
        perror("In listen"); // 打印错误信息
        exit(EXIT_FAILURE); // 退出程序
    }

    // 服务器循环等待新的连接
    while (true) {
        std::cout << "\n+++++++ Waiting for new connection ++++++++\n\n";
        // 接受新的连接请求，返回新的 socket 文件描述符
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen))<0) {
            perror("In accept"); // 打印错误信息
            exit(EXIT_FAILURE); // 退出程序
        }

        // 处理客户端请求
        serve(new_socket);
    }

    return 0;
}

