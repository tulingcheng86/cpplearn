#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <fcntl.h>

using namespace std;
const int MAX_EVENTS = 10;
const int PORT = 8080;
 
int main()
{
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    if(listen_fd==-1)
    {
        perror("socket");
        return 1;
    }

    int reuse = 1;
    if(setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse))==-1)
    {
        perror("setsockopt");
        return 1;
    }

    sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);
    if (bind(listen_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr)) == -1) 
    {
        perror("bind");
        return 1;
    }

    if(listen(listen_fd,SOMAXCONN)==-1)
    {
        perror("listen");
        return 1;
    }

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("epoll_create1");
        return 1;
    }
    
    epoll_event event;
    event.events = EPOLLIN | EPOLLET; // 边缘触发模式
    event.data.fd = listen_fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event) == -1) 
    {
        perror("epoll_ctl");
        return 1;
    }
    // 创建用于存放就绪事件的数组
    epoll_event events[MAX_EVENTS];
    cout << "Server started. Listening on port " << PORT << endl;
    while (true) 
    {
        // 等待就绪事件
        int num_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            perror("epoll_wait");
            return 1;
        }
        // 处理就绪事件
        for (int i = 0; i < num_events; ++i) {
            if (events[i].data.fd == listen_fd) {
                // 接受新连接
                sockaddr_in client_addr;
                socklen_t client_addr_len = sizeof(client_addr);
                int client_fd = accept(listen_fd, reinterpret_cast<sockaddr*>(&client_addr), &client_addr_len);
                if (client_fd == -1) {
                    perror("accept");
                    continue;
                }
                cout << "New connection from " << inet_ntoa(client_addr.sin_addr) << ":" << ntohs(client_addr.sin_port) << endl;

                // 将新连接套接字设置为非阻塞模式
                int flags = fcntl(client_fd, F_GETFL, 0);
                if (flags == -1) {
                    perror("fcntl");
                    close(client_fd);
                    continue;
                }
                flags |= O_NONBLOCK;
                if (fcntl(client_fd, F_SETFL, flags) == -1) {
                    perror("fcntl");
                    close(client_fd);
                    continue;
                }

                // 将新连接套接字添加到 epoll 实例中
                event.events = EPOLLIN | EPOLLET; // 边缘触发模式
                event.data.fd = client_fd;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event) == -1) {
                    perror("epoll_ctl");
                    close(client_fd);
                    continue;
                }
            } else {
                // 接收和回显消息
                char buffer[1024];
                ssize_t num_bytes = read(events[i].data.fd, buffer, sizeof(buffer));
                if (num_bytes == -1) {
                    perror("read");
                    close(events[i].data.fd);
                    continue;
                } else if (num_bytes == 0) {
                    // 客户端关闭连接
                    cout << "Connection closed by client" << endl;
                    close(events[i].data.fd);
                    continue;
                }

                // 将收到的消息回显给客户端
                if (write(events[i].data.fd, buffer, num_bytes) == -1) {
                    perror("write");
                    close(events[i].data.fd);
                    continue;
                }
            }
        }
    }
      

}