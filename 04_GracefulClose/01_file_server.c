#include "util_all.h"
#include <unistd.h>

#define BUF_SIZE 30

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    FILE *fp = fopen("01_file_server.c", "rb");
    if (fp == NULL) handleError(getMsgByCode(ERR_FILE_OPEN_CALL));

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);


    int ret = tcp_server_handle(argv[1], &serv, &clnt);
    if (ret != 0)   handleError(getMsgByCode(ret));

    // cntb是英文单词 count 的缩写，
    int rd_cnt = 0; // 文件读取字节数
    char buf[BUF_SIZE] = {0};
    while(1) {
        rd_cnt = fread((void*)buf, 1, BUF_SIZE, fp);
        if (rd_cnt < BUF_SIZE) {
            write(clnt.sock, buf, rd_cnt);
            break;
        }
        write(clnt.sock, buf, BUF_SIZE);
    }
    shutdown(clnt.sock, SHUT_WR); // 半关闭，传输EOF

    read(clnt.sock, buf, BUF_SIZE); // 依旧通过输入流接收数据
    printf("Message from client: %s \n", buf);

    fclose(fp);
    close(clnt.sock); close(serv.sock);
    return 0;    

}