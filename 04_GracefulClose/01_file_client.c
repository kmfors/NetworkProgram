#include "util_all.h"
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);

    FILE* fp = fopen("receive.dat", "wb");
    if (fp == NULL) handleError(getMsgByCode(ERR_FILE_OPEN_CALL));
    
    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if (ret != 0)   handleError(getMsgByCode(ret));

    char buf[BUF_SIZE] = {0};
    int read_cnt = 0;
    while((read_cnt = read(serv.sock, buf, BUF_SIZE)) != 0) {
        fwrite((void*)buf, 1, read_cnt, fp);
    }

    puts("Received file data");
    write(serv.sock, "Thank you", 10);
    fclose(fp);
    close(serv.sock);
    return 0;
}

