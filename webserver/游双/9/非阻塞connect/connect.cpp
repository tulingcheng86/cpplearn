#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>


using namespace std;

int main()
{
    int sockfd=socket(AF_INET,SOCK_STREAM,0);

    int flags=fcntl(sockfd,F_GETFL,0);
    fcntl(sockfd,F_SETFL,flags | O_NONBLOCK);

    struct sockaddr_in server_addr;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=inet_addr("127.0.0.1"); // 服务器IP地址
    server_addr.sin_port=htons(8080); // 服务器端口号

    int result=connect(sockfd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(result==0)
    {
        cout<<"connect!"<<endl;
    }
    else if(errno == EINPROGRESS)
    {
                // 连接正在进行中
        std::cout << "Connecting in progress" << std::endl;

        fd_set write_fds;
        FD_ZERO(& write_fds);
        FD_SET(sockfd,&write_fds);
        timeval timeout;
        timeout.tv_sec=3;
        timeout.tv_usec=0;

        int ret=select(sockfd+1,NULL,&write_fds,NULL,&timeout);
        if(ret>0 && FD_ISSET(sockfd, &write_fds))
        {
            cout<<"connect"<<endl;
        }
        else
        {
              cerr << "Error connecting "<< endl;
        }

    }
    else
    {
        // 连接出错
        std::cerr << "Error connecting: " << strerror(errno) << std::endl;
        close(sockfd);
        return 1;
    }
    
    // 关闭套接字
    close(sockfd);

    return 0;

}