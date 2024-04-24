#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


#define BUF_SIZE 100

void *recv_msg(void *arg);
void error_handling(char *msg);

char name[BUF_SIZE];

int main(int argc, char *argv[])
{
    int sock;
    struct sockaddr_in serv_adr;
    pthread_t rcv_thread;
    void *thread_return;

    if (argc != 4)
    {
        printf("Usage : %s <IP> <port> <name>\n", argv[0]);
        exit(1);
    }

    sprintf(name, "[%s]", argv[3]);
    //1
    sock = socket(PF_INET, SOCK_STREAM, 0);
    //2
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));
    //3
    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("connect() error");
    //4
    pthread_create(&rcv_thread, NULL, recv_msg, (void *)&sock);
    pthread_detach(rcv_thread);

    while (1)
    {
        char msg[BUF_SIZE];
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
        {
            close(sock);
            exit(0);
        }
        // 将发送者的名称与消息一起发送到服务器
        char send_msg[BUF_SIZE + BUF_SIZE];
        sprintf(send_msg, "%s %s", name, msg);
        write(sock, send_msg, strlen(send_msg));

    }
    return 0;
}

void *recv_msg(void *arg)
{
    int sock = *((int *)arg);
    char msg[BUF_SIZE];
    int str_len;

    while (1)
    {
        str_len = read(sock, msg, sizeof(msg) - 1);
        if (str_len == -1)
            return (void *)-1;
        msg[str_len] = 0;
        fputs(msg, stdout);
    }
}

void error_handling(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}
