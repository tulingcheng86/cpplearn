#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8888
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
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
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(PORT);

    // 绑定套接字
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Error: Failed to bind socket\n";
        close(sockfd);
        return 1;
    }

    std::cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        // 接收数据
        long recvLen = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (recvLen < 0) {
            std::cerr << "Error: Failed to receive data\n";
            break;
        }

        buffer[recvLen] = '\0';
        std::cout << "Received from client: " << buffer << std::endl;

        // 如果收到 "q"，则退出循环
        if (strcmp(buffer, "q") == 0) {
            std::cout << "Client disconnected.\n";
            break;
        }

        // 将接收到的消息再发送回客户端
        if (sendto(sockfd, buffer, recvLen, 0, (struct sockaddr *)&clientAddr, addrLen) < 0) 
        {
            std::cerr << "Error: Failed to send data\n";
            break;
        }
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
