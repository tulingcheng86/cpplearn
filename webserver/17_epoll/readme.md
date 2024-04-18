# epoll

### 服务器端代码原理：

1. **创建监听套接字：**
   
   ```cpp
   int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
   ```
   这行代码创建了一个 TCP 套接字，用于监听客户端连接。
   
2. **设置地址重用：**
   ```cpp
   int reuse = 1;
   setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
   ```
   这段代码设置了套接字选项，使得地址重用可用。这样做的目的是为了在服务器关闭后重新启动时能够立即重用相同的端口。

3. **绑定地址和端口：**
   ```cpp
   bind(listen_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
   ```
   这行代码将服务器的地址和端口绑定到监听套接字上，以便客户端可以连接到服务器。

4. **开始监听连接：**
   ```cpp
   listen(listen_fd, SOMAXCONN);
   ```
   这行代码开始监听连接请求，`SOMAXCONN` 参数指定了内核监听队列的长度。

5. **创建 epoll 实例：**
   ```cpp
   int epoll_fd = epoll_create1(0);
   ```
   这行代码创建了一个 epoll 实例，用于管理多个文件描述符的事件。

6. **将监听套接字添加到 epoll 实例中：**
   ```cpp
   epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event);
   ```
   这行代码将监听套接字添加到 epoll 实例中，并注册了监听读事件。

7. **主循环处理事件：**
   ```cpp
   epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
   ```
   这行代码等待就绪事件发生，一旦有事件发生，就会返回就绪事件的信息。

8. **处理新连接和数据：**
   在主循环中，通过遍历就绪事件数组，判断事件类型是新连接到达还是现有连接上有数据到达，并做出相应的处理。

### 客户端代码原理：

1. **创建套接字并连接到服务器：**
   ```cpp
   int client_fd = socket(AF_INET, SOCK_STREAM, 0);
   connect(client_fd, reinterpret_cast<sockaddr*>(&server_addr), sizeof(server_addr));
   ```
   这两行代码创建了一个 TCP 套接字，并连接到指定的服务器地址和端口。

2. **发送消息到服务器：**
   ```cpp
   send(client_fd, message.c_str(), message.length(), 0);
   ```
   这行代码将用户输入的消息发送到服务器端。

3. **接收服务器的回显消息：**
   ```cpp
   recv(client_fd, buffer, sizeof(buffer), 0);
   ```
   这行代码接收来自服务器的回显消息，并将其存储在缓冲区中。

4. **处理用户输入：**
   客户端进入一个循环，不断地等待用户输入消息，并发送到服务器端。同时，它也不断接收来自服务器的回显消息，并打印出来。当用户输入 "quit" 时，循环结束，客户端关闭套接字。

以上就是服务器端和客户端代码的工作原理。服务器端通过 epoll 监听多个连接，处理新连接和现有连接上的数据；客户端连接到服务器，并通过发送和接收消息与服务器进行通信。希望这样的解释能够帮助你更好地理解代码的运行流程。
