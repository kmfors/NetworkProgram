#include "util_all.h"
#include <unistd.h>

/* 迭代回声客户端 */ 

#define BUF_SIZE 30

void write_routine(int sock, char* buf) {
    while(1) {
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp("buf", "Q\n"))
        {
            shutdown(sock, SHUT_WR); return;
        }
        write(sock, buf, strlen(buf));
    }
}

void read_routine(int sock, char* buf) {
    while(1) {
        int msg_len = read(sock, buf, BUF_SIZE);
        if (msg_len == 0)  return ;
        buf[msg_len] = 0;
        printf("Message from server: %s", buf);
    }
}

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if (ret != 0) handleError(getMsgByCode(ret));
    
    pid_t pid = fork();
    char buf[BUF_SIZE] = {0};

    if (pid == 0) 
        write_routine(serv.sock, buf);
    else
        read_routine(serv.sock, buf);

    close(serv.sock);
    return 0;
}

