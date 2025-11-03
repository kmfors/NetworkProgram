#define _POSIX_C_SOURCE 200809L // 含 C11 特性
#include "util_all.h"
#include <fcntl.h>
#include <signal.h>

/**
 * #include <sys/socket.h>
 * ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags);
 * ssize_t recv(int sockfd, void *buf, size_t nbytes, int flags); 
 * 
 * send 与 recv 是专门为套接字设计的发送与接收函数
 * 
 * 注意以上这两个函数的 flag 字段，它允许我们指定一些特殊操作：
 *  - MSG_OOB：用于传输带外数据（Out-of-Band），即紧急消息。这允许数据接收方得到紧急通知并优先处理
 * 
 * 当收到 MSG_OOB 紧急消息时，操作系统将产生 SIGURG 信号
*/

#define BUF_SIZE 1024

int recv_sock;

void urg_handler(int signo) {
    char buf[BUF_SIZE] = {0};
    int str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s \n", buf);
}

int main(int argc, char* argv[])
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(struct sigaction, act);
    act.sa_handler = urg_handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);
    int ret = tcp_server_handle(argv[1], &serv, &clnt);
    if(ret != 0)    handleError(getMsgByCode(ret));
    recv_sock = clnt.sock;

    // fcntl 异步通知：当套接字上发生紧急事件（例如收到带外数据OOB）时，
    // 确保当前进程能收到一个 SIGURG 信号，从而能够及时做出响应和处理
    fcntl(clnt.sock, F_SETOWN, getpid());
    int state = sigaction(SIGURG, &act, 0);
    int str_len = 0;
    char buf[BUF_SIZE] = {0};
    while((str_len = recv(clnt.sock, buf, sizeof(buf), 0)) != 0) {
        if(str_len == -1)   continue;
        buf[str_len] = 0;
        puts(buf);
    }
    close(clnt.sock);
    close(serv.sock);
    return 0;
}