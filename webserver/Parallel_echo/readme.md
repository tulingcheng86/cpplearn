# 服务端

这段代码实现了一个简单的多客户端聊天服务器，使用了TCP协议进行通信。让我们逐步深入了解它：

1. 引入必要的头文件：
```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <vector>
#include <algorithm>
#include <signal.h>
```
这些头文件包含了所需的系统函数和数据结构，用于网络通信、信号处理等。

2. 定义常量：
```cpp
#define PORT 8888
#define BUFFER_SIZE 1024
```
这里定义了服务器监听的端口号和接收消息的缓冲区大小。

3. 定义全局变量：
```cpp
vector<int> client_sockets;
```
这个全局变量用于存储连接到服务器的客户端套接字。

4. 定义处理客户端连接的函数：
```cpp
void handle_client(int client_socket, sockaddr_in client_addr)
```
这个函数在新的子进程中被调用，用于处理单个客户端的消息收发。

5. 主函数：
```cpp
int main()
```
主函数包含了服务器的主要逻辑。

6. 创建套接字：
```cpp
sockfd = socket(AF_INET, SOCK_STREAM, 0);
```
通过socket()函数创建一个TCP套接字，用于监听客户端连接。

7. 绑定地址：
```cpp
bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
```
将套接字绑定到服务器地址和端口上。

8. 监听连接请求：
```cpp
listen(sockfd, 5);
```
开始监听客户端的连接请求，允许同时处理最多5个连接请求。

9. 设置信号处理程序：
```cpp
signal(SIGINT, [](int) {...});
```
设置当接收到SIGINT信号（Ctrl+C）时的处理程序，用于优雅地关闭服务器。

10. 循环处理连接：
```cpp
while (true) {...}
```
接受客户端连接请求，并为每个连接创建一个新的子进程来处理。

11. 处理客户端连接：
```cpp
accept(sockfd, (struct sockaddr *)&clientAddr, &addrLen);
```
接受客户端的连接请求，返回一个新的套接字用于与客户端通信。

12. 将客户端套接字添加到列表中：
```cpp
client_sockets.push_back(client_sockfd);
```
将新连接的套接字加入到全局的客户端套接字列表中。

13. 创建子进程处理客户端请求：
```cpp
fork();
```
通过fork()函数创建一个新的子进程来处理客户端请求。

14. 在子进程中处理客户端请求：
```cpp
handle_client(client_sockfd, clientAddr);
```
在子进程中调用handle_client函数来处理客户端的消息收发。

15. 在父进程中继续监听新的连接请求：
```cpp
close(client_sockfd);
```
在父进程中关闭已连接套接字，继续监听新的连接请求。

16. 清理已断开连接的客户端套接字：
```cpp
client_sockets.erase(remove_if(...), client_sockets.end());
```
在循环中，检查已连接套接字的状态，如果已经断开连接，则从列表中移除。

17. 关闭服务器套接字：
```cpp
close(sockfd);
```
在程序结束时关闭服务器的主套接字。

这就是这段代码的基本逻辑。它是一个简单的多客户端聊天服务器，能够接收来自客户端的消息并将其广播给其他客户端。



# 客户端

这段代码实现了一个简单的TCP客户端程序，用于连接到指定的服务器并与其进行交互。让我逐步解释一下代码的实现：

1. 导入必要的头文件：
```cpp
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <thread>
```
这些头文件包含了程序所需的基本功能，如输入输出、网络套接字编程等。

2. 定义常量：
```cpp
#define SERVER_IP "127.0.0.1"
#define PORT 8888
#define BUFFER_SIZE 1024
```
这里定义了服务器的IP地址和端口号，以及接收缓冲区的大小。

3. 定义接收消息的线程函数：
```cpp
void receive_thread(int sockfd) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;

    while (true) {
        // 接收服务器回显消息
        bytes_received = recv(sockfd, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            cout << "Disconnected from server\n";
            break;
        }

        buffer[bytes_received] = '\0';
        cout << "Echo from server: " << buffer << endl;
    }
}
```
这个函数在一个单独的线程中运行，负责接收从服务器返回的消息，并将其显示在控制台上。

4. 主函数 `main()`：
```cpp
int main() {
    int sockfd;
    struct sockaddr_in serverAddr;

    // 创建TCP套接字
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        cerr << "Error: Failed to create socket\n";
        return 1;
    }

    // 设置服务器地址
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);
    serverAddr.sin_port = htons(PORT);

    // 连接服务器
    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        cerr << "Error: Failed to connect to server\n";
        close(sockfd);
        return 1;
    }

    // 启动接收线程
    thread receive(receive_thread, sockfd);
    receive.detach(); // 分离线程，使得主线程可以继续运行

    // 与服务器交互
    string message;
    while (true) {
        cout << "Enter message (q to quit): ";
        getline(cin, message);

        // 发送消息给服务器
        send(sockfd, message.c_str(), message.size(), 0);

        // 如果输入 "q"，则退出循环
        if (message == "q") {
            break;
        }
    }

    // 关闭套接字
    close(sockfd);

    return 0;
}
```
在 `main()` 函数中：

- 创建一个TCP套接字，并检查是否成功创建。
- 设置服务器地址，并尝试连接到服务器。
- 启动一个接收线程，该线程运行 `receive_thread` 函数，负责接收服务器的消息。
- 在主线程中，通过 `getline()` 获取用户输入的消息，并将其发送给服务器。
- 如果用户输入 "q"，则退出循环。
- 关闭套接字并返回0。

这就是整个程序的实现。它允许用户与服务器建立TCP连接，并能够发送和接收消息。
