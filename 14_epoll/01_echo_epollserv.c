#include "util_all.h"
#include <unistd.h>
#include <sys/epoll.h>

/**
 * 迭代回声服务器端 - 标准IO函数实现版本
 * 
 * epoll_wait 的核心工作机制：
 *  - 阻塞等待：当没有事件时，线程会休眠，不消耗CPU
 *  - 批量返回：一次调用返回所有就绪的事件
 *  - 高效处理：避免了遍历所有监控的文件描述符
 *  - 就绪事件数组：ep_events 只包含真正有事件发生的fd
 * 
 * 
*/

#define BUF_SIZE 1024
#define EPOLL_SIZE 50

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    char buf[BUF_SIZE] = {0};

    // 1. 创建一个epoll实例
    int epfd = epoll_create(EPOLL_SIZE); 

    // 2. 分配事件数组，存储就绪事件；EPOLL_SIZE监控文件描述符数量（自行动态调整）
    struct epoll_event *ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    // 3. 配置监控事件
    struct epoll_event event;
    event.events = EPOLLIN; // 监控可读事件
    event.data.fd = serv.sock; // 关联套接字

    // 4. 注册事件到 epoll（将套接字添加到 epoll 监控中，并关联一个epoll实例）
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv.sock, &event); 

    int event_cnt = 0; // 事件计数器
    int str_len = 0;
    while(1) {
        // 5. 无限期（-1）阻塞等待 epoll 实例上关联的多个监控事件发生，若触发：
        // 将就绪事件，放置于就绪事件数组中
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) {perror("epoll_wait"); break;}

        for (int i = 0; i < event_cnt; i++) {
            if (ep_events[i].data.fd == serv.sock) {
                clnt.addr_len = sizeof(clnt.addr);
                clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);

                event.events = EPOLLIN;
                event.data.fd = clnt.sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt.sock, &event);
                printf("Connected client socket=%d \n", clnt.sock);
            } else {
                str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                if (str_len == 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(event.data.fd);
                    printf("closed client: %d \n", ep_events[i].data.fd);
                } else {
                    write(ep_events[i].data.fd, buf, str_len); // echo !
                }
            }
        }
    }
    close(serv.sock);
    close(epfd);
    free(ep_events);
    return 0;
}
