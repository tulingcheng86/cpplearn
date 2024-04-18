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

void broadcastSender() {
    // 创建UDP套接字
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置广播选项
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 构造广播地址结构
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(BROADCAST_PORT);
    //255.255.255.255 huo INADDR_BROADCAST   
    //addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    inet_pton(AF_INET,"255.255.255.255",&addr.sin_addr);
    
    std::string message = "Hello, broadcast world!";

    // 发送广播消息
    if (sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }

    std::cout << "Broadcast message sent." << std::endl;

    close(sock);
}

int main() {
    broadcastSender();
    return 0;
}
