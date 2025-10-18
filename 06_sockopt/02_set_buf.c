#include "util_all.h"
#include <unistd.h>

/**
 * SO_SNDBUF - 发送缓冲区，控制套接字发送数据的缓冲区大小
 * 
 * SO_RCVBUF - 接收缓冲区，控制套接字接收数据的缓冲区大小
 * 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>

int main(int argc, char* argv[]) {
    int state = 0;  // 设置或获取套接字函数的返回状态
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    int snd_buf, rcv_buf;
    socklen_t len = sizeof(snd_buf);
    
    // 获取当前缓冲区大小
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state)  handleError(getMsgByCode(ERR_GET_SOCKOPT_FUNC_CALL));
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state)  handleError(getMsgByCode(ERR_GET_SOCKOPT_FUNC_CALL));
    
    printf("默认发送缓冲区大小: %d bytes\n", snd_buf);
    printf("默认接收缓冲区大小: %d bytes\n", rcv_buf);
    
    // 设置新的缓冲区大小
    int new_size = 1024 * 32;  // 32KB
    state = setsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&new_size, sizeof(new_size));
    if (state)  handleError(getMsgByCode(ERR_SET_SOCKOPT_FUNC_CALL));
    state = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&new_size, sizeof(new_size));
    if (state)  handleError(getMsgByCode(ERR_SET_SOCKOPT_FUNC_CALL));
    
    // 验证设置结果
    state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, (void*)&snd_buf, &len);
    if (state)  handleError(getMsgByCode(ERR_GET_SOCKOPT_FUNC_CALL));
    state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, (void*)&rcv_buf, &len);
    if (state)  handleError(getMsgByCode(ERR_GET_SOCKOPT_FUNC_CALL));
    
    printf("设置后发送缓冲区大小: %d bytes\n", snd_buf);
    printf("设置后接收缓冲区大小: %d bytes\n", rcv_buf);
    
    close(sock);
    return 0;
}

