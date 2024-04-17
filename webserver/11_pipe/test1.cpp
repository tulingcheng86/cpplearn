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
int main() {
    int pipefd[2];
    pid_t pid;

    // 创建管道
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    // 创建子进程
    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // 子进程
        close(pipefd[1]); // 关闭写入端

        // 从父进程接收消息
        char buffer[255];
        ssize_t bytes_read = read(pipefd[0], buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("read");
            return 1;
        }

        std::cout << "Child received: " << buffer << std::endl;

        // 发送响应给父进程
        const char* response = "Message from Child";
        ssize_t bytes_written = write(pipefd[0], response, strlen(response));
        if (bytes_written == -1) {
            perror("write");
            return 1;
        }

        close(pipefd[0]); // 关闭读取端
    } else { // 父进程
        close(pipefd[0]); // 关闭读取端

        // 发送消息给子进程
        const char* message = "Hello Child Process!";
        ssize_t bytes_written = write(pipefd[1], message, strlen(message));
        if (bytes_written == -1) {
            perror("write");
            return 1;
        }

        // 从子进程接收响应
        char buffer[255];
        ssize_t bytes_read = read(pipefd[1], buffer, sizeof(buffer));
        if (bytes_read == -1) {
            perror("read");
            return 1;
        }

        std::cout << "Parent received: " << buffer << std::endl;

        close(pipefd[1]); // 关闭写入端
    }

    return 0;
}
