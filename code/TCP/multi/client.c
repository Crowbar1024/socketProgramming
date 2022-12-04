#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main() {

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        perror("socket");
        exit(-1);
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    inet_pton(AF_INET, "192.168.193.128", &serveraddr.sin_addr.s_addr);
    serveraddr.sin_port = htons(9999);
    int ret = connect(fd, (struct sockaddr*) &serveraddr, sizeof(serveraddr));
    if(ret == -1) {
        perror("connect");
        exit(-1);
    }
    
    char recvBuf[1024];
    int i = 0;
    while(1) {
        sprintf(recvBuf, "data : %d\n", i++);
        write(fd, recvBuf, strlen(recvBuf)+1);  // 因为strlen不会统计'\n'

        int len = read(fd, recvBuf, sizeof(recvBuf));
        if (len == -1) {
            perror("read");
            exit(-1);
        } else if (len > 0) {
            printf("recv server : %s\n", recvBuf);
        } else if (len == 0) {
            printf("server closed...");
            break;
        }
        sleep(1);
    }

    close(fd);

    return 0;
}