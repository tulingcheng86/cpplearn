#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;
const int PORT = 8080;
const char* SERVER_IP = "127.0.0.1";

int main()
{
    // 创建套接字
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) 
    {
        perror("socket");
        return 1;
    }
    // 设置服务器地址
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);
    // 连接到服务器
    if (connect(client_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        perror("connect");
        close(client_fd);
        return 1;
    }
    cout << "Connected to server" << endl;
    while(true)
    {
        // 发送消息到服务器
        cout << "Enter message(q is break): ";
        string message;
        getline(cin, message);
        if (message == "q") break;

        if (send(client_fd, message.c_str(), message.length(), 0) == -1) 
        {
            perror("send");
            close(client_fd);
            return 1;
        }

        // 接收服务器的回显消息
        char buffer[1024];
        ssize_t num_bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (num_bytes == -1) {
            perror("recv");
            close(client_fd);
            return 1;
        } 
        else if (num_bytes == 0) {
            cout << "Server closed connection" << endl;
            close(client_fd);
            return 0;
        }

        cout << "Server says: " << string(buffer, num_bytes) << endl;
    }

}
