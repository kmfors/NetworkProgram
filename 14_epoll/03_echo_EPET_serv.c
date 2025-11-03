#include "util_all.h"
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/epoll.h>

/**
 * 
 * 水平触发（Level Trigger）：又称条件触发，只要输入缓冲有数据，就会一直通知该事件；
 *  - 所以只要输入缓冲还剩有数据，就仍然会注册该事件方式
 * 
 * 边缘触发（Edge Trigger）：输入缓冲收到数据时，仅注册一次事件，即使输入缓冲中还留有数据，也不会再进行注册。
 * 
 * Linux声明了一个全局变量 int errno; 在error.h头文件中，因此每种函数发生错误时，会将错误值拷贝到errno中。
 *  - read函数发现输入缓冲无数据可读，返回-1，同时在 errno 中保存 EAGAIN 常量
 * 
 * 更改或读取文件属性方法：
 *  - int fcntl(int files, int cmd, ...);
 * 
 * 所以若将文件（套接字）改为非阻塞模式，只需要2条语句：
 *  - int flag = fcntl(fd, F_GETFL, 0);
 *  - fcntl(fd, F_SETFL, flag|O_NONBLOCK);
 * 
 * 注意：边缘触发模式下，以阻塞方式工作的 read与write函数有可能引起服务端长时间的停顿。为什么？
 *  - ET模式下只通知一次，必须一次读完所有数据！
 *  - 阻塞操作会等待IO完成，期间无法处理其他事件
 * 
 *  因此，解决方案是，边缘触发一定要采用非阻塞的read&write函数。
 * 
 * 
 * 边缘触发实例如下：
*/

#define BUF_SIZE 4 // 减少缓冲区大小，避免一次性读取全部数据
#define EPOLL_SIZE 50

void set_nonblock_mode(int fd) {
    int flag = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flag|O_NONBLOCK);
}

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    int epfd = epoll_create(EPOLL_SIZE); 
    struct epoll_event *ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    set_nonblock_mode(serv.sock); // 改为非阻塞，提高连接并发

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
        puts("edge trigger: return epoll_wait");

        for (int i = 0; i < event_cnt; i++) {
            if (ep_events[i].data.fd == serv.sock) {
                clnt.addr_len = sizeof(clnt.addr);
                clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
                set_nonblock_mode(serv.sock); // 改为非阻塞
                 
                event.events = EPOLLIN|EPOLLET; // 边缘触发
                event.data.fd = clnt.sock;
                epoll_ctl(epfd, EPOLL_CTL_ADD, clnt.sock, &event);
                printf("Connected client socket=%d \n", clnt.sock);
            } else {
                while(1) {
                    str_len = read(ep_events[i].data.fd, buf, BUF_SIZE);
                    if (str_len == 0) {
                        epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                        close(event.data.fd);
                        printf("closed client: %d \n", ep_events[i].data.fd);
                        break;
                    } else if (str_len < 0){
                        // 判断输入缓冲已读完，终结循环非堵塞读
                        if (errno == EAGAIN)  break;
                    } else {
                        write(ep_events[i].data.fd, buf, str_len); // echo !
                    }
                }
                
            }
        }
    }
    close(serv.sock);
    close(epfd);
    free(ep_events);
    return 0;
}
