/* 基于多进程的回声服务器端 */

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
    struct sigaction act;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int state = sigaction(SIGCHLD, &act, 0);

    // socket 前两个参数即可确定，可填 0 省略第三个参数
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

    pid_t pid;
    char msg[BUF_SIZE];
    int clnt_sock = 0, msg_len = 0;
    InitField(struct sockaddr_in, clnt_addr);
    socklen_t clnt_addr_sz = sizeof(clnt_addr);

    while(1) {
        clnt_addr_sz = sizeof(clnt_addr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        if (clnt_sock == -1)  continue;

        puts("new client connected...");
        pid = fork();
        if (pid == -1)  {
            close(clnt_sock);
            continue;
        }

        if (pid == 0) {
            close(serv_sock); // 子进程不需要拷贝而来的serv_sock,关闭
            while((msg_len = read(clnt_sock, msg, BUF_SIZE)) != 0)
                write(clnt_sock, msg, msg_len);
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