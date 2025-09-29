/* 基于process_serv/08_echo_mpserv.c 的拓展
*  该示例可以跟任意的回声客户端配合运行
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define InitField(Type, field) Type field;memset(&field, 0, sizeof(field))

void error_handling(char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void read_childproc(int sig) {
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed proc id: %d \n", id);
    }
}

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    // 注册信号处理函数
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int state = sigaction(SIGCHLD, &act, 0);

    // 设置网络监听
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)  error_handling("socket() error");

    InitField(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)  error_handling("bind() error");

    ret = listen(serv_sock, 5);
    if (ret == -1)  error_handling("listen() error");

    // 设置客户端地址信息与数据信息;
    char msg[BUF_SIZE];
    int clnt_sock = 0, msg_len = 0;
    InitField(struct sockaddr_in, clnt_addr);
    socklen_t clnt_addr_sz = sizeof(clnt_addr);

    // 设置管道与拷贝父进程
    int fds[2];
    pipe(fds);
    pid_t pid = fork();
    if (pid == 0) {
        int len= 0;
        char msgbuf[BUF_SIZE] = "hello,world";
        FILE* fp = fopen("echomsg", "wt");
        // 从管道出口读到缓冲区msgbuf
        for (int i = 0; i < 10; i++) {
            len = read(fds[0], msgbuf, BUF_SIZE);
            // 将缓冲区msgbuf的内容写到文件中
            fwrite((void*)msgbuf, 1, len, fp);
        }
        fclose(fp);
        return 0; // 销毁子进程
    }

    while(1) {
        clnt_addr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if (clnt_sock == -1)  continue;

        puts("new client connected...");
        pid = fork();
        if (pid == 0) {
            close(serv_sock); // 子进程不需要拷贝而来的serv_sock,关闭
            while((msg_len = read(clnt_sock, msg, BUF_SIZE)) != 0) {
                write(clnt_sock, msg, msg_len);
                write(fds[1], msg, msg_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        } else {
            close(clnt_sock); // 父进程不需要做读写，关闭并交由子进程来做
        }
    }
    close(serv_sock);
    return 0;
}

/*
* 随着程序的运行，我们可以清晰的看到，每当一个客户端连接时，都会fork一个子进程.
* 而每一个客户端的结束，都会将对应的fork的子进程进行关闭。
*/