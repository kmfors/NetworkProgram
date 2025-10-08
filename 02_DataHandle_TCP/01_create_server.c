#include "util_all.h"
#include <unistd.h>

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_server_handle(argv[1], &serv, &clnt);
    if (ret != 0) {
        handleError(getMsgByCode(ret));
    }

    char msg[] = "hello,world!";
    // 应该向客户端连接套接字写入
    write(clnt.sock, msg, sizeof(msg));

    close(clnt.sock);
    close(serv.sock);
    return 0;
}
