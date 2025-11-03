#include "util_all.h"

/**
 * 多播（Multicast）的UDP消息接收客户端，它加入指定的多播组并接收发送端广播的新闻消息。
 * 
 *  
*/
#define BUF_SIZE 30
int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
    INIT_STRUCT_FIELD(serv_sock_info_t, recv);
    int ret = udp_server_handle(argv[2], &recv);
    if(ret != 0) handleError(getMsgByCode(ret));

    INIT_STRUCT_FIELD(struct ip_mreq, join_adr);
    join_adr.imr_multiaddr.s_addr = inet_addr(argv[1]); // 多播组地址
    join_adr.imr_interface.s_addr = htonl(INADDR_ANY); // 任意网络接口

    setsockopt(recv.sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_adr, sizeof(join_adr));
    
    int str_len = 0;
    char buf[BUF_SIZE] = {0};
    while(1) {
        str_len = recvfrom(recv.sock, buf, BUF_SIZE-1, 0, NULL, 0);
        if(str_len < 0) break;
        buf[str_len] = 0;
        fputs(buf, stdout);
    }
    close(recv.sock);
    return 0;
}