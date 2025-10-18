#include "util_all.h"

/**
 * 之前的程序都是创建好套接字后（未经特别操作）直接使用的，此时通过默认的套接字特性进行数据通信。
 * 
 * 套接字的多种可选项，不同的协议层内有着不同的可选项
 * 
 * 获取socket选项，成功返回0， 失败返回-1
 * int getsockopt(int sock, int level, int optname, void* optval, socklen_t* optlen);
 *  - level：要查看的可选项的协议层，
 *  - optname：要查看的可选项名
 * 
 * 设置socket可选项，成功返回0， 失败返回-1
 * int setsockopt(int sock, int level, int optname, const void* optval, socklen_t optlen);
 * 
 * 分别创建两个套接字，获取各个套接字的类型信息
*/

// 选取协议层为 SOL_SOCKET （通用套接字层选项）
int main(int argc, char* argv[]) {

    int sock_type = 0;
    socklen_t optlen = sizeof(sock_type);
    int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    int udp_sock = socket(PF_INET, SOCK_DGRAM, 0);
    
    printf("SOCK_STREAM: %d \n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d \n", SOCK_DGRAM);

    int state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if (state)  handleError(getMsgByCode(ERR_GET_SOCKOPT_FUNC_CALL));
    printf("Socket type one: %d \n", sock_type);

    state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (void*)&sock_type, &optlen);
    if (state)  handleError(getMsgByCode(ERR_GET_SOCKOPT_FUNC_CALL));
    printf("Socket type two: %d \n", sock_type);

    return 0;
}
