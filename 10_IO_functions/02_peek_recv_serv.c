#include "util_all.h"

/**
 * MSG_PEEK：窥探，这个标志让 recv 从套接字的接收缓冲区中读取数据，但不会将这些数据从缓冲区中移除。
 * 
 * MSG_DONTWAIT：非阻塞。这个标志让本次 recv 调用变为非阻塞的。如果接收缓冲区中没有立即可用的数据，
 * 函数不会阻塞（暂停）等待，而是立即返回一个错误。
 * 
 * 当这两个标志通过按位或（|）组合在一起使用时，它们产生的效果是：
 * 立即查看一下接收缓冲区里有什么数据，但不管看没看到，都马上返回，不要等待。
 * 
*/
#define BUF_SIZE 30
int main(int argc, char* argv[])
{
    ASSERT_ARGC_SERVER(argc);
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);
    int ret = tcp_server_handle(argv[1], &serv, &clnt);
    if(ret != 0)    handleError(getMsgByCode(ret));

    int str_len = 0;
    char buf[BUF_SIZE] = {0};
    while(1) {
        str_len = recv(clnt.sock, buf, sizeof(buf)-1, MSG_PEEK|MSG_DONTWAIT);
        if(str_len > 0) break;
    }
    buf[str_len] = 0;
    printf("Buffering %d bytes: %s \n", str_len, buf);

    str_len = recv(clnt.sock, buf, sizeof(buf)-1, 0);
    buf[str_len] = 0;
    printf("Read again: %s \n", buf);

    close(serv.sock);
    close(clnt.sock);
    return 0;
}