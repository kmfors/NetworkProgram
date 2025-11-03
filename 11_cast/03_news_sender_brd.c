#include "util_all.h"

/**
 *  ./sender 255.255.255.255 9190
 * 广播（Broadcast）的UDP消息发送客户端，程序读取 "news.txt" 文件中的内容，以广播形式发送到整个局域网的所有主机。
 * 
*/

#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, send);
    int ret = udp_client_handle(argv[1], argv[2], &send);
    if (ret != 0) handleError(getMsgByCode(ret));

    int so_brd = 1;
    setsockopt(send.sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));

    FILE* fp = fopen("news.txt", "r");
    if(fp == NULL)  handleError("fopen error");
    
    char buf[BUF_SIZE] = {0};
    while(!feof(fp)) {
        fgets(buf, BUF_SIZE, fp);
        sendto(send.sock, buf, strlen(buf), 0, (struct sockaddr*)&send.addr, send.addr_len);
        sleep(2);
    }
    fclose(fp);
    close(send.sock);
    return 0;
}