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

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)  error_handling("UDP socket creation error");

    InitField(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    int ret = bind(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)  error_handling("bind() error");

    int msg_len = 0;
    socklen_t clnt_addr_sz;
    char msg[BUF_SIZE];
    InitField(struct sockaddr_in, clnt_addr);
    while(1) {
        clnt_addr_sz = sizeof(clnt_addr);
        msg_len = recvfrom(sock, msg, BUF_SIZE, 0, (struct sockaddr*)&clnt_addr, &clnt_addr_sz);
        sendto(sock, msg, msg_len, 0, (struct sockaddr*)&clnt_addr, clnt_addr_sz);
    }
    close(sock);
    return 0;
}