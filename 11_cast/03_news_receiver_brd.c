#include "util_all.h"

/**
 * 程序创建一个UDP服务器，绑定到指定端口，持续接收发送到该端口的单播消息并显示在屏幕上。
 *  ./rece 9190
*/
#define BUF_SIZE 30
int main(int argc, char* argv[])
{
    ASSERT_ARGC_SERVER(argc);
    INIT_STRUCT_FIELD(serv_sock_info_t, recv);
    int ret = udp_server_handle(argv[1], &recv);
    if(ret != 0) handleError(getMsgByCode(ret));

    int str_len = 0;
    char buf[BUF_SIZE] = {0};
    while(1) {
        str_len = recvfrom(recv.sock, buf, BUF_SIZE-1, 0, NULL, 0);
        if(str_len < 0) break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    close(recv.sock);
    return 0;
}