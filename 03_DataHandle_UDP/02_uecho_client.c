#include "util_all.h"
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = udp_client_handle(argv[1], argv[2], &clnt);
    if (ret != 0)   handleError(getMsgByCode(ret));

    char buf[BUF_SIZE] = {0};
    while(1) {
        fputs("Inputs msg(Q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))  { break; }
        
        sendto(clnt.sock, buf, strlen(buf), 0, (struct sockaddr*)&clnt.sock, clnt.addr_len)
        write(clnt.sock, buf, strlen(buf));
        size = read(clnt.sock, buf, BUF_SIZE - 1);
        //size = read(clnt.sock, buf, sizeof(buf) - 1);

        buf[size] = 0; // 字符串以 \0 结尾
        printf("msg from server: %s", buf);
    }


    return 0;
}