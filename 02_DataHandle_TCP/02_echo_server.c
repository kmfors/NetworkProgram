#include "../util_tool.h"
#include "../util_error.h"
#include "../util_sock.h"
#include <unistd.h>

/**
 * 迭代回声服务器端
 * 
*/

#define BUF_SIZE 1024

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_server_listen(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    int size = 0;
    char buf[BUF_SIZE];
    for (int i = 0; i < 50; i++) {
        clnt.socket = accept(serv.socket, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        if (clnt.socket == -1)  handleError(getMsgByCode(1004));
        
        printf("Connected client %d \n", i + 1);
        while ((size = read(clnt.socket, buf, BUF_SIZE)) != 0) 
            write(clnt.socket, buf, size);
    }

    close(clnt.socket);
    close(serv.socket);
    return 0;
}
