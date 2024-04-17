#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main()
{
    int server_fd,client_fds[MAX_CLIENTS],max_clients = MAX_CLIENTS;
    struct sockaddr_in server_addr, client_addr;
    fd_set reads, temps;
    char buffer[BUFFER_SIZE];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }

    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 绑定服务器地址
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return 1;
    }

    // 监听连接
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        return 1;
    }

    std::cout << "Server started. Waiting for connections..." << std::endl;
    // 初始化客户端套接字集合

    FD_ZERO(&reads);
    FD_SET(server_fd,&reads);

    for (int i = 0; i < max_clients; i++) 
    {
        client_fds[i] = 0;
    }

    while(true)
    {
        temps=reads;
        int activity=select(FD_SETSIZE,&temps,nullptr,nullptr,nullptr);
        if (activity == -1) {
            perror("select");
            return 1;
        }

        if(FD_ISSET(server_fd,&temps))
        {
            socklen_t addr_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
            if (client_fd == -1) 
            {
                perror("accept");
                return 1;
            }
            std::cout << "New connection, socket fd is " << client_fd << ", IP is " << inet_ntoa(client_addr.sin_addr) << ", port is " << ntohs(client_addr.sin_port) << std::endl;
            // 将新的客户端套接字添加到套接字集合中
            for (int i = 0; i < max_clients; i++) 
            {
                if (client_fds[i] == 0) {
                    client_fds[i] = client_fd;
                    break;
                }
            }
            // 添加新的客户端套接字到文件描述符集合
            FD_SET(client_fd, &reads);
        }
        // 处理客户端发来的数据
        for (int i = 0; i < max_clients; i++)
        {
            int client_fd = client_fds[i];
            if (FD_ISSET(client_fd,&temps))
            {
                int bytes_read = read(client_fd, buffer, BUFFER_SIZE);
                if (bytes_read == 0) {
                    // 客户端关闭连接
                    std::cout << "Client disconnected, socket fd is " << client_fd << std::endl;
                    close(client_fd);
                    FD_CLR(client_fd, &reads);
                    client_fds[i] = 0;
                } else {
                    // 接收到数据，回显给客户端
                    write(client_fd, buffer, bytes_read);
                }               
            }
         
        }

    }
    return 0;
}