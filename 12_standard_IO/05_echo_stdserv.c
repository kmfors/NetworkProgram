#include "util_all.h"
#include <unistd.h>
#include <errno.h>

/**
 * 迭代回声服务器端 - 标准IO函数实现版本
 * 
 * 注意：fflush(writefp); 标准IO函数为了提高性能，内部提供额外的缓冲，因此，若不调用fflush函数
 * 则无法保证立即将数据传输到客户端
 * 
*/

#define BUF_SIZE 1024

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    char buf[BUF_SIZE];
    FILE *iofp;
    int client_count = 0;

    while (1) {
        clnt.addr_len = sizeof(clnt.addr);
        clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        if (clnt.sock == -1) {
            // 信号打断并返回 -1 且置 errno=EINTR
            if (errno == EINTR) continue;
            handleError(getMsgByCode(1004));
        }
        printf("Connected client %d \n", ++client_count);

        iofp = fdopen(clnt.sock, "r+");
        if (iofp == NULL) {
            close(clnt.sock);
            clnt.sock = -1;
            continue;
        }

        while (fgets(buf, BUF_SIZE, iofp) != NULL) {
            if (fputs(buf, iofp) == EOF) break;
            if (fflush(iofp) == EOF) break;
        }

        fclose(iofp); // closes underlying clnt.sock as well
        clnt.sock = -1;
    }

    close(serv.sock);
    return 0;
}
