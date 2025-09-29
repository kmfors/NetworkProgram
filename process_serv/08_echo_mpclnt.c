/* 迭代回声客户端 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
#define InitField(Type, field) Type field;memset(&field, 0, sizeof(field))

void error_handling(char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

void write_routine(int sock, char* msg) {
    while(1) {
        fgets(msg, BUF_SIZE, stdin);
        if(!strcmp(msg, "q\n") || !strcmp("msg", "Q\n"))
        {
            shutdown(sock, SHUT_WR);
        }
        write(sock, msg, strlen(msg));
    }
}

void read_routine(int sock, char* msg) {
    while(1) {
        int msg_len = read(sock, msg, BUF_SIZE);
        if (msg_len == 0)  return ;
        msg[msg_len] = 0;
        printf("Message from server: %s", msg);
    }
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }
    // socket 前两个参数即可确定，可填 0 省略第三个参数
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)  error_handling("socket() error");

    InitField(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    int ret = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)  error_handling("connect() error!");
    
    pid_t pid = fork();
    char msg[BUF_SIZE];
    if (pid == 0)  write_routine(sock, msg);
    else  read_routine(sock, msg);

    close(sock);
    return 0;
}

