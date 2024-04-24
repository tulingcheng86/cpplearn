#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/select.h>

using namespace std;

int main() {
    // 创建 TCP 套接字
    int tcp_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_sockfd < 0) {
        cerr << "Error creating TCP socket" << endl;
        return 1;
    }

    // 创建 UDP 套接字
    int udp_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (udp_sockfd < 0) {
        cerr << "Error creating UDP socket" << endl;
        close(tcp_sockfd);
        return 1;
    }

    // 设置 TCP 地址和端口
    struct sockaddr_in tcp_server_addr;
    tcp_server_addr.sin_family = AF_INET;
    tcp_server_addr.sin_addr.s_addr = INADDR_ANY;
    tcp_server_addr.sin_port = htons(8080);

    // 设置 UDP 地址和端口
    struct sockaddr_in udp_server_addr;
    udp_server_addr.sin_family = AF_INET;
    udp_server_addr.sin_addr.s_addr = INADDR_ANY;
    udp_server_addr.sin_port = htons(8080);

    // 绑定 TCP 地址和端口
    if (bind(tcp_sockfd, (struct sockaddr*)&tcp_server_addr, sizeof(tcp_server_addr)) < 0) {
        cerr << "Error binding TCP address" << endl;
        close(tcp_sockfd);
        close(udp_sockfd);
        return 1;
    }

    // 绑定 UDP 地址和端口
    if (bind(udp_sockfd, (struct sockaddr*)&udp_server_addr, sizeof(udp_server_addr)) < 0) {
        cerr << "Error binding UDP address" << endl;
        close(tcp_sockfd);
        close(udp_sockfd);
        return 1;
    }

    // 监听 TCP 套接字
    if (listen(tcp_sockfd, 5) < 0) {
        cerr << "Error listening on TCP socket" << endl;
        close(tcp_sockfd);
        close(udp_sockfd);
        return 1;
    }

    cout << "Server started. Listening on port 8080 for TCP and UDP connections..." << endl;

    // 设置文件描述符集合
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(tcp_sockfd, &fds);
    FD_SET(udp_sockfd, &fds);

    while (true) {
        // 复制文件描述符集合
        fd_set read_fds = fds;

        // 调用 select() 函数
        int max_fd = max(tcp_sockfd, udp_sockfd);
        int ret = select(max_fd + 1, &read_fds, nullptr, nullptr, nullptr);
        if (ret < 0) {
            cerr << "Error in select()" << endl;
            break;
        }

        // 检查 TCP 套接字是否可读
        if (FD_ISSET(tcp_sockfd, &read_fds)) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int client_sockfd = accept(tcp_sockfd, (struct sockaddr*)&client_addr, &client_len);
            if (client_sockfd < 0) {
                cerr << "Error accepting TCP connection" << endl;
                break;
            }
            cout << "TCP connection accepted from " << inet_ntoa(client_addr.sin_addr) << endl;
            // 在此处处理 TCP 连接
            close(client_sockfd); // 这里只是示例，实际应用中需要处理客户端请求
        }

        // 检查 UDP 套接字是否可读
        if (FD_ISSET(udp_sockfd, &read_fds)) {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            char buffer[1024];
            ssize_t bytes_received = recvfrom(udp_sockfd, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &client_len);
            if (bytes_received < 0) {
                cerr << "Error receiving UDP data" << endl;
                break;
            }
            cout << "UDP data received from " << inet_ntoa(client_addr.sin_addr) << ": " << string(buffer, bytes_received) << endl;
            // 在此处处理 UDP 数据
        }
    }

    // 关闭套接字
    close(tcp_sockfd);
    close(udp_sockfd);

    return 0;
}
