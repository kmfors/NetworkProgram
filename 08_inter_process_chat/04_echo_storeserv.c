#include "util_all.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


/**
 * 保存消息的回声服务器
 * 
 * 因为在每次调用accept之后，clnt_addr_sz 会被修改为实际使用的地址结构体的大小。
 * 但是，在下次循环时，我们再次调用accept之前，需要将clnt_addr_sz重置为clnt_addr结构体的实际大小，
 * 因为accept函数要求传入的addrlen参数在输入时必须是地址结构体的长度。
 * 不然的话，下一次循环时 clnt_addr_sz 可能包含错误的值。
 * 
 * 多进程服务器的缺点就是需要大量的运算和内存空间，相互之间的数据交换也相对复杂
*/

#define BUF_SIZE 1024

void read_childproc(int sig) {
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed proc id: %d \n", id);
    }
}

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    // 注册信号处理函数
    INIT_STRUCT_FIELD(struct sigaction, act);
    act.sa_flags = 0;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    int state = sigaction(SIGCHLD, &act, 0);

    // 设置网络监听
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);
    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    // 设置客户端地址信息与数据信息
    char buf[BUF_SIZE];
    ssize_t str_len = 0;

    // 设置管道
    int fds[2] = {0};
    if (pipe(fds) != 0) return 1;

    pid_t pid = fork();
    // 子进程将从管道里读取的数据写入文件中
    if (pid == 0) { 
        close(fds[1]);  // 关闭不需要的写入端
        
        ssize_t len = 0;
        char msgbuf[BUF_SIZE] = {0};
        FILE* fp = fopen("echo_msg", "wt");
        if (fp == NULL) {
            perror("fopen failed");
            exit(1);
        }
        
        // 持续读取直到管道关闭
        while ((len = read(fds[0], msgbuf, BUF_SIZE)) > 0) {
            fwrite((void*)msgbuf, 1, len, fp);
            fflush(fp);  // 确保数据写入磁盘
        }
        
        fclose(fp);
        close(fds[0]);
        exit(0);
    } else {
        close(fds[0]);  // 父进程关闭不需要的读取端
    }

    while(1) {
        clnt.addr_len = sizeof(clnt.addr);
        clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        if (clnt.sock == -1) continue;

        puts("new client connected...");
        pid = fork();
        if (pid == 0) {
            // 子进程处理客户端连接
            close(serv.sock);
            while((str_len = read(clnt.sock, buf, BUF_SIZE)) != 0) {
                write(clnt.sock, buf, str_len);  // 回声
                write(fds[1], buf, str_len);     // 写入管道
            }
            close(clnt.sock);
            puts("client disconnected...");
            exit(0);
        } else {
            close(clnt.sock);  // 父进程关闭客户端socket
        }
    }
    
    close(fds[1]);  // 关闭管道写入端
    close(serv.sock);
    return 0;
}

/*
* 随着程序的运行，我们可以清晰的看到，每当一个客户端连接时，都会fork一个子进程.
* 而每一个客户端的结束，都会将对应的fork的子进程进行关闭。
*/