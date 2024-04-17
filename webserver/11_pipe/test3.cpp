#include <iostream>
#include <unistd.h>

#define BUF_SIZE 30

int main()
{
    int fds1[2],fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds1);
    pipe(fds2);

    pid=fork();

    //son
    if(pid==0)
    {
        read(fds1[0], buf, BUF_SIZE);
        std::cout << "Child proc output: " << buf << std::endl;
        write(fds2[1], str2, sizeof(str2));

    }
    else
    {
        write(fds1[1], str1, sizeof(str1));

        read(fds2[0], buf, BUF_SIZE);
        std::cout << "Parent proc output: " << buf << std::endl;

    }

return 0;
}