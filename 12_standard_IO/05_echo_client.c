#include "util_all.h"
#include <unistd.h>

/**
 * 迭代回声客户端 - 标准IO函数实现版本
 * 
*/

#define BUF_SIZE 1024

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
    
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);

    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if (ret != 0) {
        handleError(getMsgByCode(ret));
    }
    printf("Connect Success! \n");

    char buf[BUF_SIZE] = {0};
    FILE* readfp = fdopen(serv.sock, "r");
    FILE* writefp = fdopen(serv.sock, "w");
    while(1) {
        fputs("Inputs msg(Q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))  { break; }
        
        fputs(buf, writefp);
        fflush(writefp);
        fgets(buf, BUF_SIZE, readfp);
        printf("msg from server: %s", buf);
    }
    close(serv.sock);
    return 0;
}


