#include "util_all.h"

/**
 * 多播（Multicast）的UDP消息发送客户端，它从一个文本文件中读取新闻内容，然后以多播的形式定期发送出去。
 * 
 * 接收者需要加入特定的多播组（地址范围：224.0.0.0 - 239.255.255.255），例如 224.1.1.2 9190
 * 
 * 
*/
#define TTL 64
#define BUF_SIZE 30

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, send);
    int ret = udp_client_handle(argv[1], argv[2], &send);
    if (ret != 0) handleError(getMsgByCode(ret));

    int time_live = TTL;
    setsockopt(send.sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));

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