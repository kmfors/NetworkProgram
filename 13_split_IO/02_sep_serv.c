#include "util_all.h"
#include <unistd.h>

#define BUF_SIZE 1024
int main(int argc, char* argv[])
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_server_handle(argv[1], &serv, &clnt);
    if (ret != 0) handleError(getMsgByCode(ret));

    FILE* readfp = fdopen(clnt.sock, "r");
    FILE* writefp = fdopen(dup(clnt.sock), "w"); // 拷贝文件描述符

    fputs("FROM SERVER: Hi~ client? \n", writefp);
    fputs("I love all of the world \n", writefp);
    fputs("You are awesome! \n", writefp);
    fflush(writefp);

    // 半关闭，传输EOF；关闭输出流，但保留输入流
    shutdown(fileno(writefp), SHUT_WR);
    fclose(writefp);

    char buf[BUF_SIZE] = {0};
    fgets(buf, BUF_SIZE, readfp);fputs(buf, stdout);
    fclose(readfp);
    return 0;
}