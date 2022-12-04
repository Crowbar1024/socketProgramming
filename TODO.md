



READ 返回0的处理不同



负责监听的socket为什么要用下面的循环来接受呢

```C++
while((accept_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_addr_len)) > 0)
```

由于accept是阻塞的，所以每次有新的进来，应该会停在那里

解答：由于listen_fd设置的是非阻塞，并且是epoll_et模式，循环到空就结束了








检查所有超时的定时器时会对定时器队列加锁

如果此时某个负责http请求和回复的socket又活跃了，想要更新时间，就得加锁，此时被阻塞了



epoll一般先创建自己的实例，然后添加一个用于监听的lfd，然后定义一个epoll_event数组，用epoll_wait去监听，然后把变化的存进数组。

可以发现，并没有add这个数组。在wait前这个数组是空的。







非静态函数可以调用静态成员，但静态函数‘只能’直接访问静态成员，要访问非静态得传参数。




当得到一个完整、正确的HTTP请求时，我们就分析目标文件的属性， 如果目标文件存在、对所有用户可读，且不是目录，则使用mmap将其映射到内存地址m_file_address处，并告诉调用者获取文件成功。然后将这个地址存一下， 写HTTP响应时用writev(m_sockfd, m_iv, m_iv_count);或者其他写函数写到客户端

// 创建内存映射是否需要强行转换呢

( char* )

static_cast(

写回文件时是否需要做完整性检查

但我的版本是while1的

keep alive设置的化，需要初始化请求方法、请求内容等等，但没有初始化sockfd和address等信息，因为这些不变。

但不管有没有设置keep alive，都需要重置socket上的EPOLLONESHOT事件（当然你得有这个需求）

epoll_event是个结构体，当然可以用数组或指针





有些版本的服务器，把HTTP请求任务加入到线程池之前，会把对应的计时器干掉。




开启EPOLLONESHOT，针对客户端连接的描述符，listenfd不用开启

linyacool把accept的fd和listen的fd在得到后直接就设置成非阻塞的了。而qin则是只在addfd之后设置。

linyacool 对于epoll_del也是得设置最后参数event，但qin直接赋值0，我觉得后者更简单。

两者对重置EPOLLONESHOT的操作很奇怪。前者在modfd里写。后者在外部设置event时加入。

前者把主逻辑封装成了一个类，感觉不伦不类。


struct linger tmp = {1, 1};
        setsockopt(m_listenfd, SOL_SOCKET, SO_LINGER, &tmp, sizeof(tmp));


int optval = 1;
    if(setsockopt(listen_fd, SOL_SOCKET,  SO_REUSEADDR, &optval, sizeof(optval)) == -1)
        return -1;


#### HTTP服务器的域名解析

有些没有pathname，其实就是index目录