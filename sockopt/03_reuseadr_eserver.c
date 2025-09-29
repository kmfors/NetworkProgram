#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define TRUE 1
#define FALSE 0
#define InitField(Type, field) Type field;memset(&field, 0, sizeof(field))

void error_handling(char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)  error_handling("socket() error");

    /*  解决地址分配错误
    int option = TRUE;
    socklen_t optlen = sizeof(option);
    setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
    */

    InitField(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret)  error_handling("bind() error");

    ret = listen(serv_sock, 5);
    if (ret == -1)  error_handling("listen error");

    InitField(struct sockaddr_in, clnt_addr);
    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
    int msg_len = 0;
    char msg[30];
    while((msg_len = read(clnt_sock, msg, sizeof(msg))) != 0) {
        write(clnt_sock, msg, msg_len);
        write(1, msg, msg_len);
    }
    close(clnt_sock);
    close(serv_sock);
    return 0;
}