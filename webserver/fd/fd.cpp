#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    // 打开一个文件，返回文件描述符
    int fd = open("example.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 打印文件描述符
    printf("File descriptor: %d\n", fd);

    // 关闭文件
    close(fd);
    
    return 0;
}
