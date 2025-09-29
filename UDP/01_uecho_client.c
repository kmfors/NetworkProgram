/* 未连接套接字的数据传输，可向不同目标传输数据 */

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
    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)  error_handling("UDP socket creation error");

    InitField(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    int msg_len = 0;
    socklen_t addr_sz;
    char msg[BUF_SIZE];
    InitField(struct sockaddr_in, from_addr);
    while(1) {
        fputs("Insert msg(q to quit): ", stdout);
        fgets(msg, sizeof(msg), stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))  break;

        sendto(sock, msg, strlen(msg), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

        addr_sz = sizeof(from_addr); 
        msg_len = recvfrom(sock, msg, BUF_SIZE, 0, (struct sockaddr*)&from_addr, &addr_sz);
        msg[msg_len] = 0;
        printf("msg from server: %s", msg);
    }
    close(sock);
    return 0;
}