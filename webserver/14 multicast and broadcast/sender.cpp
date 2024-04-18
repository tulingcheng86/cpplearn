#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <chrono>
#include <unistd.h>

const int MULTICAST_PORT = 8888;
const char *MULTICAST_GROUP = "239.0.0.1";

void sender() {
    // 创建 UDP 套接字
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 设置套接字为允许发送广播
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 构造广播地址结构
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(MULTICAST_PORT);
    inet_pton(AF_INET, MULTICAST_GROUP, &addr.sin_addr);

    std::string message = "Hello, multicast world!";

    // 发送消息
    while (true) {
        if (sendto(sock, message.c_str(), message.length(), 0, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("sendto");
            exit(EXIT_FAILURE);
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    close(sock);
}

int main() {
    sender();
    return 0;
}
