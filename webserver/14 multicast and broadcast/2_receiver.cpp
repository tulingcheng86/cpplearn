#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <chrono>
#include <unistd.h>

const int BROADCAST_PORT = 8888;

void broadcastReceiver() {
    // 创建UDP套接字
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 构造本地地址结构
    struct sockaddr_in local_addr;
    memset(&local_addr, 0, sizeof(local_addr));
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(BROADCAST_PORT);
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // 绑定套接字到本地地址
    if (bind(sock, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    struct sockaddr_in sender_addr;
    socklen_t sender_addr_len = sizeof(sender_addr);

    // 接收广播消息
    int recv_len = recvfrom(sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&sender_addr, &sender_addr_len);
    if (recv_len < 0) {
        perror("recvfrom");
        exit(EXIT_FAILURE);
    }
    buffer[recv_len] = '\0';

    std::cout << "Received broadcast message: " << buffer << std::endl;

    close(sock);
}

int main() {
    broadcastReceiver();
    return 0;
}
