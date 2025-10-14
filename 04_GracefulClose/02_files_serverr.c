#include "util_all.h"
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    FILE *fp = fopen("file_server.c", "rb");
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);


    int ret = tcp_server_handle(argv[2], &serv, &clnt);
    if (ret != 0)   handleError(getMsgByCode(ret));

    // cnt是英文单词count的缩写，
    int rd_cnt = 0; // 文件读取字节数
    char buf[BUF_SIZE] = {0};
    while(1) {
        rd_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (rd_cnt < BUF_SIZE) {
            write(serv.sock, buf, rd_cnt);
            break;
        }
        write(serv.sock, buf, BUF_SIZE);
    }
    shutdown(serv.sock, SHUT_WR);    

    read(serv.sock, buf, BUF_SIZE);
    printf("msg from client: %s \n", buf);

    fclose(fp);
    close(clnt_sock);
    close(sock);
    return 0;    

    
}