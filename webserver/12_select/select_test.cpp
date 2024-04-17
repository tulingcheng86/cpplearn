#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 30

int main(int argc, char *argv[]) {
    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads); // 0 is standard input (console)

    while (true) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        result = select(1, &temps, nullptr, nullptr, &timeout);
        if (result == -1) {
            std::cout << "select() error!" << std::endl;
            break;
        } else if (result == 0) {
            std::cout << "Time-out!" << std::endl;
        } else {
            if (FD_ISSET(0, &temps)) {
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = '\0'; // null-terminate the string
                std::cout << "Message from console: " << buf << std::endl;
            }
        }
    }

    return 0;
}
