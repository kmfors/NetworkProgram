#include "../util_tool.h"
#include "../util_error.h"
#include "../util_sock.h"
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    ASSERT_ARGC_SERVER(argc);

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