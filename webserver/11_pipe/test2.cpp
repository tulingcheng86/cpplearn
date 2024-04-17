#include <iostream>
#include <unistd.h>

#define BUF_SIZE 30

int main() {
    int fds1[2], fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds1);
    pipe(fds2);

    pid = fork();

    if (pid == 0) { // Child process
        close(fds1[1]);
        close(fds2[0]);

        read(fds1[0], buf, BUF_SIZE);
        std::cout << "Child proc output: " << buf << std::endl;

        write(fds2[1], str2, sizeof(str2));
    } else { // Parent process
        close(fds1[0]);
        close(fds2[1]);

        write(fds1[1], str1, sizeof(str1));

        read(fds2[0], buf, BUF_SIZE);
        std::cout << "Parent proc output: " << buf << std::endl;

        sleep(3);
    }

    return 0;
}
