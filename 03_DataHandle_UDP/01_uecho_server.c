#include "util_all.h"
#include <unistd.h>

/**
 * 
 * 在UDP服务端与客户端的通信中：
 *  - 接收来源地址：通过 recvfrom 获取的地址 = 客户端的地址
 *  - 发送目标地址：在 sendto 中指定的地址 = 同一个客户端的地址
 * 
 * 在C语言的UDP编程中，服务端接收到的客户端地址信息中的端口号，
 *  是一个由客户端的操作系统自动分配的、临时的、不确定的端口号。
 * 
 * UDP接收完整的数据报，因此缓冲区里存放整个数据报，取缓冲区最大值，否则大小不足会截断
 * 而TCP是接收数据流的一部分，因此它的缓冲区存放流数据的一段，可以多次接收
 * 
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    int ret = udp_server_handle(argv[1], &serv);
    if (ret != 0)  handleError(getMsgByCode(ret));

    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);
    ssize_t str_len = 0;
    char buf[BUF_SIZE] = {0};
    while(1) {
        clnt.addr_len = sizeof(clnt.addr);
        str_len = recvfrom(serv.sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        sendto(serv.sock, buf, str_len, 0, (struct sockaddr*)&clnt.addr, clnt.addr_len);
    }
    close(serv.sock);
    return 0;
}