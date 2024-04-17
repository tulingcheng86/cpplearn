# select

当我们希望等待某个或某些文件描述符上的事件发生时，可以使用 `select()` 函数。这个函数允许我们指定一个文件描述符集合，然后等待这些文件描述符中的任何一个或多个变得可读、可写或出现异常。下面是对代码中 `select()` 函数的使用进行详细的讲解：

1. 首先，我们创建了两个 `fd_set` 类型的变量 `reads` 和 `temps`。`fd_set` 是一个用来表示文件描述符集合的数据结构，在这里我们用它来表示我们感兴趣的文件描述符。我们将 `reads` 设置为初始的文件描述符集合。

```cpp
fd_set reads, temps;
FD_ZERO(&reads);
FD_SET(0, &reads); // 0 表示标准输入（console）
```

2. 在一个无限循环中，我们调用 `select()` 函数来等待标准输入上是否有数据可读。在每次循环中，我们将 `temps` 设置为当前感兴趣的文件描述符集合的拷贝，以便 `select()` 函数在等待期间不会修改原始集合。

```cpp
while (true) {
    temps = reads;
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;
```

3. 然后，我们调用 `select()` 函数，它会等待指定的文件描述符集合中的任何一个变得可读或直到超时。这个函数会返回一个整数值，表示有多少个文件描述符变得可读。

```cpp
result = select(1, &temps, nullptr, nullptr, &timeout);
```

- 第一个参数 `1` 表示我们要监听的文件描述符的数量（在这个例子中只有标准输入，所以是1）。
- 第二个参数 `&temps` 是一个指向 `fd_set` 类型的指针，表示我们要监听的文件描述符集合。
- 第三、第四个参数 `nullptr` 表示我们不关心文件描述符的可写和异常事件。
- 第五个参数 `&timeout` 是一个 `timeval` 结构体，表示超时时间。在这里，我们设置了超时时间为 5 秒。

4. 如果 `select()` 函数返回值是 `-1`，表示出现了错误，我们输出错误信息并退出循环。如果返回值是 `0`，表示超时，我们输出提示信息，然后继续下一轮循环。如果返回值大于 `0`，表示有事件发生，我们继续检查标准输入上是否有数据可读。

```cpp
if (result == -1) {
    std::cout << "select() error!" << std::endl;
    break;
} else if (result == 0) {
    std::cout << "Time-out!" << std::endl;
} else {
    // 处理文件描述符上的事件
}
```

5. 在处理文件描述符上的事件时，我们首先检查标准输入上是否有数据可读。如果有数据可读，我们使用 `read()` 函数从标准输入读取数据，并将读取到的数据输出到控制台。

```cpp
if (FD_ISSET(0, &temps)) {
    str_len = read(0, buf, BUF_SIZE);
    buf[str_len] = '\0'; // 在字符串末尾添加 NULL 结束符
    std::cout << "Message from console: " << buf << std::endl;
}
```

在这个例子中，我们只监听了标准输入上的事件，所以 `FD_ISSET(0, &temps)` 检查的是标准输入是否在 `temps` 集合中。如果是，表示标准输入上有数据可读，我们就读取数据并输出到控制台。



# I/O复用服务器

逐步详细讲解服务端和客户端的代码。

### 1. 服务端代码（echo_server.cpp）

首先，让我们从服务端代码开始：

```cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
```

首先，我们引入了必要的头文件，包括用于网络编程的 `<sys/socket.h>` 和 `<arpa/inet.h>`，以及用于 I/O 复用的 `<sys/select.h>`。然后，我们定义了一些常量，包括服务器端口号、最大客户端数量以及消息缓冲区的大小。

接下来是 `main()` 函数：

```cpp
int main() {
    int server_fd, client_fds[MAX_CLIENTS], max_clients = MAX_CLIENTS;
    struct sockaddr_in server_addr, client_addr;
    fd_set reads, temps;
    char buffer[BUFFER_SIZE];
```

我们声明了一些变量，包括服务器套接字 `server_fd`、客户端套接字数组 `client_fds`、最大客户端数量 `max_clients`，以及用于存储客户端地址的结构体变量 `client_addr`。我们还声明了两个 `fd_set` 类型的变量 `reads` 和 `temps`，分别用于存储感兴趣的文件描述符集合和临时的文件描述符集合。最后，我们声明了一个用于接收数据的缓冲区 `buffer`。

```cpp
    // 创建服务器套接字
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return 1;
    }
```

我们创建了服务器套接字，并检查是否创建成功。如果创建失败，我们输出错误信息并退出程序。

```cpp
    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
```

接下来，我们设置了服务器地址。我们指定了协议族为 `AF_INET`，IP 地址为 `INADDR_ANY`，表示接受来自任何网络接口的连接，端口号为我们定义的常量 `PORT`。

```cpp
    // 绑定服务器地址
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        return 1;
    }
```

然后，我们将服务器套接字绑定到指定的地址。如果绑定失败，我们输出错误信息并退出程序。

```cpp
    // 监听连接
    if (listen(server_fd, 5) == -1) {
        perror("listen");
        return 1;
    }
```

接下来，我们监听连接。`listen()` 函数用于将服务器套接字转换为被动套接字，等待客户端连接。参数 `5` 表示可以同时处理的最大连接请求数。

```cpp
    std::cout << "Server started. Waiting for connections..." << std::endl;

    // 初始化客户端套接字集合
    FD_ZERO(&reads);
    FD_SET(server_fd, &reads);

    for (int i = 0; i < max_clients; i++) {
        client_fds[i] = 0;
    }
```

我们打印出提示信息，表示服务器已启动，并正在等待连接。然后，我们**初始化客户端套接字集合，并将服务器套接字添加到集合中**。接着，我们**初始化客户端套接字数组，将所有元素初始化为 `0`**。

以上是服务器端的初始化部分，接下来是服务器端的主循环，用于接受和处理客户端连接。

```cpp
    while (true) {
        temps = reads;
        int activity = select(FD_SETSIZE, &temps, nullptr, nullptr, nullptr);
        if (activity == -1) {
            perror("select");
            return 1;
        }
```

在主循环中，我们首先**将 `temps` 设置为当前感兴趣的文件描述符集合的拷贝**，并**调用 `select()` 函数等待事件发生**。`select()` 函数返回的 `activity` 表示有多少个文件描述符变得可读。

```cpp
        // 检查服务器套接字是否有新连接
        if (FD_ISSET(server_fd, &temps)) {
            socklen_t addr_len = sizeof(client_addr);
            int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
            if (client_fd == -1) {
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
```

然后，我们**检查服务器套接字是否有新的连接请求**。如果有新的连接请求，我们调用 `accept()` 函数接受连接，并获取客户端的地址信息。然后，我们打印连接信息，并将新的客户端套接字添加到套接字集合和客户端套接字数组中。

```cpp
        // 处理客户端发来的数据
        for (int i = 0; i < max_clients; i++) {
            int client_fd = client_fds[i];
            if (FD_ISSET(client_fd, &temps)) {
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
```

最后，我们处理客户端发送的数据。我们遍历客户端套接字数组，检查每个客户端套接字是否有数据可读。如果有数据可读，我们读取数据并进行处理。如果客户端关闭连接，我们关闭套接字并从集合和数组中移除。否则，我们将接收到的数据回显给客户端。

### 2. 客户端代码（echo_client.cpp）

接下来，让我们看一下客户端的代码：

```cpp
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
```

客户端的代码也需要引入必要的头文件，包括用于网络编程的 `<sys/socket.h>` 和 `<arpa/inet.h>`。

```cpp
#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024
```

然后，我们定义了一些常量，包括服务器 IP 地址、端口号以及消息缓冲区的大小。

```cpp
int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
```

接下来，我们声明了一些变量，包括客户端套接字 `client_fd`、服务器地址 `server_addr`，以及用于存储数据的缓冲区 `buffer`。

```cpp
    // 创建客户端套接字
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket");
        return 1;
    }
```

在 `main()` 函数中，我们创建了客户端套接字，并检查是否创建成功。

```cpp
    // 设置服务器地址
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) == -1) {
        perror("inet_pton");
        return 1;
    }
```

接下来，我们设置了服务器的地址信息。我们指定了协议族为 `AF_INET`，端口号为我们定义的常量 `PORT`，服务器 IP 地址为我们定义的常量 `SERVER_IP`。

```cpp
    // 连接到服务器
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        return 1;
    }
```

然后，我们调用 `connect()` 函数连接到服务器。如果连接失败，我们输出错误信息并退出程序。

```cpp
    // 从标准输入读取输入，并发送给服务器
    while (true) {
        std::cout << "Enter message: ";
        std::cin.getline(buffer, BUFFER_SIZE);
        int bytes_sent = send(client_fd, buffer, strlen(buffer), 0);
        if (bytes_sent == -1) {
            perror("send");
            break;
        }
```

接下来，我们进入一个循环，用于从标准输入读取输入

，并将输入的消息发送给服务器。我们使用 `send()` 函数发送数据，并检查发送是否成功。

```cpp
        // 接收服务器回显的数据
        int bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received == -1) {
            perror("recv");
            break;
        }
        buffer[bytes_received] = '\0'; // 添加 NULL 结束符
        std::cout << "Server echoed: " << buffer << std::endl;
    }
```

最后，我们接收服务器回显的数据，并显示在控制台上。我们使用 `recv()` 函数接收数据，并检查接收是否成功。如果成功，我们将接收到的数据显示在控制台上。

以上就是客户端代码的详细讲解。

这两部分代码一起构成了一个简单的基于 I/O 复用的回声服务器和客户端。服务器等待客户端连接并处理回声请求，而客户端连接到服务器并发送消息，然后接收并显示来自服务器的回声消息。
