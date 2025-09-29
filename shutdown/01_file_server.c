#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30

#define InitField(Type, field) Type field;memset(&field, 0, sizeof(field))

void error_handling(char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    FILE* fp = fopen("01_file_server.c", "rb");

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)  error_handling("socket() error");

    InitField(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)  error_handling("bind() error");

    ret = listen(sock, 5);
    if (ret == -1)  error_handling("listen() error");

    InitField(struct sockaddr_in, clnt_addr);
    socklen_t clnt_addr_sz = sizeof(clnt_addr);
    int clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, &clnt_addr_sz);

    int read_cnt = 0; // 文件读取字节数
    char buf[BUF_SIZE] = {0};
    while(1) {
        read_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (read_cnt < BUF_SIZE) {
            write(clnt_sock, buf, read_cnt);
            break;
        }
        write(clnt_sock, buf, BUF_SIZE);
    }
    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, buf, BUF_SIZE);
    printf("msg from client: %s \n", buf);

    fclose(fp);
    close(clnt_sock);
    close(sock);
    return 0;    
}