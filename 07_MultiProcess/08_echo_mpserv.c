#include "util_all.h"
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


#define BUF_SIZE 30
void read_childproc(int sig) {
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Remove proc id: %d, it's return:%d \n", id, WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);
    
    INIT_STRUCT_FIELD(struct sigaction, act)
    act.sa_flags = 0;
    act.sa_handler = read_childproc;
    sigemptyset(&act.sa_mask);
    
    int state = sigaction(SIGCHLD, &act, 0);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    pid_t pid;
    char buf[BUF_SIZE];
    ssize_t str_len = 0;

    while(1) {
        clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        if (clnt.sock == -1)  continue;

        puts("new client connected...");
        pid = fork();
        if (pid == -1)  { close(clnt.sock); continue;}
        if (pid != 0) { close(clnt.sock); continue; } // 父进程不需要做读写，关闭并交由子进程来做

        // 子进程运行区域
        close(serv.sock); // 子进程不需要拷贝而来的serv_sock,关闭
        while((str_len = read(clnt.sock, buf, BUF_SIZE)) != 0) {
            write(clnt.sock, buf, str_len);
            close(clnt.sock);
            puts("client disconnected...");
            return 0;
        } 
    }
    close(serv.sock);
    return 0;
}

/*
* 随着程序的运行，我们可以清晰的看到，每当一个客户端连接时，都会fork一个子进程.
* 而每一个客户端的结束，都会将对应的fork的子进程进行关闭。
*/