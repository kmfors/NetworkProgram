#include "util_all.h"
#include <unistd.h>

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
    FILE *readfp, *writefp;

    for (int i = 0; i < 50; i++) {
        clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        if (clnt.sock == -1)  handleError(getMsgByCode(1004));
        printf("Connected client %d \n", i + 1);

        readfp = fdopen(clnt.sock, "r");
        writefp = fdopen(clnt.sock, "w");

        while(!feof(readfp)) {
            fgets(buf, BUF_SIZE, readfp);
            fputs(buf, writefp);
            fflush(writefp);
        }
        fclose(readfp);
        fclose(writefp);
    }

    close(clnt.sock);
    close(serv.sock);
    return 0;
}
