#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <signal.h>

using namespace std;
#define PORT 8888
#define BUFFER_SIZE 1024

vector<int> client_sockets;

void handle_client(int client_socket, sockaddr_in client_addr) 
{
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    cout << "Client connected: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;

    while (true) {
        // 接收客户端消息
        bytes_received = recv(client_socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            cout << "Client disconnected: " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Message from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << ": " << buffer << endl;

        // 广播消息给其他客户端
        for (int sock : client_sockets) {
            if (sock != client_socket) {
                send(sock, buffer, bytes_received, 0);
            }
        }
    }

    // 关闭套接字
    close(client_socket);
    exit(0);
}

int main() {
    int sockfd, client_sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // 创建TCP套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
    {
        cerr << "Error: Failed to create socket\n";
        return 1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 绑定地址
    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error: Failed to bind socket\n";
        close(sockfd);
        return 1;
    }

    // 监听连接
    if (listen(sockfd, 5) < 0) {
        cerr << "Error: Failed to listen\n";
        close(sockfd);
        return 1;
    }

    cout << "Server listening on port " << PORT << "...\n";

    // 设置信号处理程序，以便在收到 SIGINT 信号时优雅地关闭服务器
    signal(SIGINT, [](int) {
        cout << "Received SIGINT. Server shutting down." << endl;
        for (int sock : client_sockets) {
            close(sock);
        }
        exit(0);
    });

    while (true) {
        // 接受连接
        client_sockfd = accept(sockfd, (struct sockaddr *)&clientAddr, &addrLen);
        if (client_sockfd < 0) {
            cerr << "Error: Failed to accept connection\n";
            continue;
        }

        // 将客户端套接字添加到列表中
        client_sockets.push_back(client_sockfd);

        // 创建子进程处理客户端请求
        pid_t pid = fork();
        if (pid < 0) 
        {
            cerr << "Error: Failed to fork\n";
            close(client_sockfd);
            continue;
        } 
        else if (pid == 0) 
        {
            // 子进程处理客户端请求
            close(sockfd);
            handle_client(client_sockfd, clientAddr);
            return 0;
        } 
        else 
        {
            // 父进程继续监听新的连接请求
            close(client_sockfd);
        }

        // 清理已经断开连接的客户端套接字
        client_sockets.erase(remove_if(client_sockets.begin(), client_sockets.end(),
                                             [](int sock) {
                                                 int error = 0;
                                                 socklen_t len = sizeof(error);
                                                 return getsockopt(sock, SOL_SOCKET, SO_ERROR, &error, &len) < 0;
                                             }),
                             client_sockets.end());
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
