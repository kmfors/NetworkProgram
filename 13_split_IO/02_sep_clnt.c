#include "util_all.h"
#include <unistd.h>

#define BUF_SIZE 1024
int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if (ret != 0) handleError(getMsgByCode(ret));

    FILE* readfp = fdopen(serv.sock, "r");
    FILE* writefp = fdopen(serv.sock, "w");

    char buf[BUF_SIZE] = {0};
    while(1) {
        if(fgets(buf, BUF_SIZE, readfp) == NULL) break;;
        fputs(buf, stdout);
        fflush(stdout);
    }

    fputs("FROM CLIENT: Thank you! \n", writefp);
    fflush(writefp);

    fclose(writefp); fclose(readfp);
    return 0;
}