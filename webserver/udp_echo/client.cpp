#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // 创建UDP套接字
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error: Failed to create socket\n";
        return 1;
    }

    // 设置服务器地址
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    while (true) {
        // 输入消息
        std::cout << "Enter message (q to quit): ";
        std::cin.getline(buffer, BUFFER_SIZE);

        // 发送数据
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
            std::cerr << "Error: Failed to send data\n";
            break;
        }

        // 如果输入 "q"，则退出循环
        if (strcmp(buffer, "q") == 0) {
            std::cout << "Disconnected from server.\n";
            break;
        }

        // Receive echo from server
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, NULL, NULL);
        if (bytes_received < 0) {
            std::cerr << "Error reading from socket\n";
            return EXIT_FAILURE;
        }

        std::cout << "Echo from server: " << buffer << std::endl;
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
