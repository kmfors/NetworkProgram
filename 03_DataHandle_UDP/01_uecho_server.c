#include "util_all.h"
#include <unistd.h>

/**
 * 成功时返回传输的字节数，失败时返回-1
 * ssize_t sendto(int sock, void* buff, size_t nbytes, int flags, 
 *      struct sockaddr* to, socklen_t addrlen);
 *  - flags：可选项参数，若没有则传递0
 *  - to：目标地址信息的地址值
 * 
 * 成功时返回接收的字节数，失败时返回-1
 * ssize_t recvfrom(int sock, void* buff, size_t nbytes, int flags, 
 *      struct sockaddr* from , socklen_t addrlen);
 * 
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    int ret = udp_server_handle(argv[1], &serv);
    if (ret != 0)  error_handling(getMsgByCode(ret));

    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);
    ssize_t str_len = 0;
    char msg[BUF_SIZE] = {0};
    while(1) {
        clnt.addr_len = sizeof(clnt.addr);
        str_len = recvfrom(serv.sock, msg, BUF_SIZE, 0, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        sendto(serv.sock, msg, str_len, 0, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
    }
    close(serv.sock);
    return 0;
}