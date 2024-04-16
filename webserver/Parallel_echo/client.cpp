#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>

using namespace std;
#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024

void receive_thread(int sockfd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while (true) {
        // 接收服务器回显消息
        bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            cout << "Disconnected from server\n";
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Echo from server: " << buffer << endl;
    }
}

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;

    // 创建TCP套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error: Failed to create socket\n";
        return 1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // 连接服务器
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error: Failed to connect to server\n";
        close(sockfd);
        return 1;
    }

    // 启动接收线程
    thread receive(receive_thread, sockfd);
    receive.detach(); // 分离线程，使得主线程可以继续运行

    // 与服务器交互
    string message;
    while (true) {
        cout << "Enter message (q to quit): ";
        getline(cin, message);

        // 发送消息给服务器
        send(sockfd, message.c_str(), message.size(), 0);

        // 如果输入 "q"，则退出循环
        if (message == "q") {
            break;
        }
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
