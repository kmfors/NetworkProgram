#include "util_all.h"

/**
 * 检查输入缓冲区
*/

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);

    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if(ret != 0)    handleError(getMsgByCode(ret));

    write(serv.sock, "123", strlen("123"));

    close(serv.sock);
    return 0;
}