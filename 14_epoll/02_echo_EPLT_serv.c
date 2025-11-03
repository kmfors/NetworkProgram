#include "util_all.h"
#include <unistd.h>
#include <sys/epoll.h>

/**
 * 
 * 水平触发（Level Trigger）：又称条件触发，只要输入缓冲有数据，就会一直通知该事件；
 *  - 所以只要输入缓冲还剩有数据，就仍然会注册该事件方式
 * 
 * 边缘触发（Edge Trigger）：输入缓冲收到数据时，仅注册一次事件，即使输入缓冲中还留有数据，也不会再进行注册。
 * 
 * 水平触发实例如下：
*/

#define BUF_SIZE 4 // 减少缓冲区大小，避免一次性读取全部数据
#define EPOLL_SIZE 50

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    int epfd = epoll_create(EPOLL_SIZE); 
    struct epoll_event *ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);
    struct epoll_event event;
    event.events = EPOLLIN; // 监控可读事件
    event.data.fd = serv.sock; // 关联套接字
    epoll_ctl(epfd, EPOLL_CTL_ADD, serv.sock, &event); 

    int str_len = 0;
    int event_cnt = 0; // 就绪事件计数
    char buf[BUF_SIZE] = {0};
    while(1) {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) {perror("epoll_wait"); break;}
        puts("level trigger: return epoll_wait");

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
