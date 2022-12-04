#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

// 因为是LT，为了体现每次都会通知的机制，让接受的字符串数组很短
// 这样每次都不能把输入缓冲区的内容全部放进去。

int main() {

    // 创建socket
    int lfd = socket(PF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr;
    saddr.sin_port = htons(9999);
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    // 绑定
    bind(lfd, (struct sockaddr *)&saddr, sizeof(saddr));

    // 监听
    listen(lfd, 8);

    int epfd = epoll_create(100);  // 创建一个fd来唯一标识内核中的epoll事件表
    struct epoll_event epevs[1024];  // 用于存储epoll事件表中就绪事件

    // 主线程往epoll内核事件表中注册监听socket事件，当listen到新的客户连接时，lfd变为就绪事件
    struct epoll_event epev;
    epev.events = EPOLLIN;
    epev.data.fd = lfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &epev);

    while(1) {
        // 主线程等待一组文件描述符上的事件，并将当前所有就绪的epoll_event复制到epevs数组中
        int ret = epoll_wait(epfd, epevs, 1024, -1);
        if(ret == -1) {
            perror("epoll_wait");
            exit(-1);
        }

        printf("ret = %d\n", ret);

        for(int i = 0; i < ret; i++) {
            int curfd = epevs[i].data.fd;  // 事件表中就绪的socket文件描述符
            if(curfd == lfd) {
                // 当listen到新的用户连接，lfd上则产生就绪事件
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd, (struct sockaddr *)&cliaddr, &len);

                epev.events = EPOLLIN;
                epev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &epev);
            } else {
                // 建议else if 
                if(epevs[i].events & EPOLLOUT) {
                    continue;
                }
                // 有数据到达，需要通信
                // char buf[5] = {0};  // 模拟LT
                int len = read(curfd, buf, sizeof(buf));
                if(len == -1) {
                    perror("read");
                    exit(-1);
                } else if(len == 0) {
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                } else if(len > 0) {
                    printf("read buf = %s\n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }

            }

        }
    }

    close(lfd);
    close(epfd);
    return 0;
}