#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

#define InitField(Type, field) Type field;memset(&field, 0, sizeof(field))

void error_handling(char* msg) {
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}

// 将协议层设为SOL_SOCKET
int main(int argc, char* argv[]) {

    int sock_type = 0;
    socklen_t optlen = sizeof(sock_type);
    int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    int udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    
    printf("SOCK_STREAM: %d \n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);

    int state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if (state)  error_handling("getsockopt() error!");
    printf("Socket type one: %d \n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if (state)  error_handling("getsockopt() error!");
    printf("Socket type two: %d \n", sock_type);

    return 0;
}
