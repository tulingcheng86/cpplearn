#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // 创建客户端套接字
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        return 1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) == -1) {
        perror("inet_pton");
        return 1;
    }

    // 连接到服务器
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }

    // 从标准输入读取输入，并发送给服务器
    while (true) {
        std::cout << "Enter message(q is break):   ";
        std::cin.getline(buffer, BUFFER_SIZE);
        int bytes_sent = send(client_fd, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            perror("send");
            break;
        }
        // 如果输入 "q"，则退出循环
        if (strcmp(buffer,"q")==0) 
        {
            break;
        }
        // 接收服务器回显的数据
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv");
            break;
        }
        buffer[bytes_received] = '\0'; // 添加 NULL 结束符
        std::cout << "Server echoed: " << buffer << std::endl;
    }
    // 关闭套接字
    close(client_fd);

    return 0;
}
