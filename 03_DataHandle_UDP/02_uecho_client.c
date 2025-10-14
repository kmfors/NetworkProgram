#include "util_all.h"
#include <unistd.h>

/**
 * UDP的服务端与客户端，只需要一个套接字即可，他们之间不需要连接。
 * 一个UDP套接字可以向任意主机传输数据。 
 * 
 * UDP客户端只与一个特定的服务器通信：
 *  - 发送目标地址：你在 sendto 中指定的服务器地址（IP和端口）。
 *  - 接收来源地址：你通过 recvfrom 获取的地址，也就是服务器回复你时所用的地址。
 * 在正常情况下，这两个地址是同一个服务器的地址。
 * 
 * 注意：
 *  1. 由于UDP客户端的常见设置是不启用connect函数的，所以无论是sendto还是recvfrom都需要指定地址
 *  2. 若UDP客户端启用connect函数，则可以使用 send 与 read 函数（不需要指定地址）
 *  3. 调用 sendto 函数时，自动分配IP和端口号给套接字
*/

#define BUF_SIZE 1024

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);
    
    int ret = udp_client_handle(argv[1], argv[2], &serv);
    if (ret != 0)   handleError(getMsgByCode(ret));

    ssize_t rsize = 0;
    char buf[BUF_SIZE] = {0};

    while(1) {
        fputs("Inputs Message(Q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))  { break; }
        
        sendto(serv.sock, buf, strlen(buf), 0, (struct sockaddr*)&serv.addr, serv.addr_len);
        clnt.addr_len = sizeof(clnt.addr);
        rsize = recvfrom(serv.sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clnt.addr,
                         &clnt.addr_len);

        buf[rsize] = 0;
        printf("Message clnt server: %s", buf);
    }
    close(serv.sock);
    return 0;
}