#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

    // 1.创建一个用于监听的socket
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if (lfd == -1) {
        perror("socket");
        exit(-1);
    }

    // 2.和本地的IP和端口进行绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    // 给socket的IP地址赋值要考虑本机到网络的转换
    // 1. 使用inet_pton
    // 2. 直接赋值（对象要做转换）
    // inet_pton(AF_INET, "192.168.193.128", saddr.sin_addr.s_addr);
    saddr.sin_addr.s_addr = INADDR_ANY;  // 0.0.0.0
    saddr.sin_port = htons(9999);
    int ret = bind(lfd, (struct sockaddr*) &saddr, sizeof(saddr));
    if (ret == -1) {
        perror("bind");
        exit(-1);
    }

    // 3.监听
    ret = listen(lfd, 8);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    // 4.接收客户端连接
    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);
    int cfd = accept(lfd, (struct sockaddr*) &clientaddr, &len);
    if (cfd == -1) {
        perror("accept");
        exit(-1);
    }
    // 输出客户端的信息
    char clientIP[16];
    inet_ntop(AF_INET, &clientaddr.sin_addr.s_addr, clientIP, sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d\n", clientIP, clientPort);

    // 5.通信
    char recvBuf[1024] = {0};
    while (1) {
        int num = read(cfd, recvBuf, sizeof(recvBuf));
        // read 见 system/IO 的基础函数
        if (num == -1) {
            perror("read");
            exit(-1);
        } else if (num > 0) {
            printf("recv client data : %s\n", recvBuf);
        } else if (num == 0) {
            // 表示客户端断开连接
            printf("clinet closed...");
            break;
        }
        char * data = "hello,i am server";
        // 给客户端发送数据
        write(cfd, data, strlen(data));
    }
   
    // 6.关闭文件描述符
    close(cfd);
    close(lfd);

    return 0;
}