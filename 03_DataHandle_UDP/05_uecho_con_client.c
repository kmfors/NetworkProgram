#include "util_all.h"
#include <unistd.h>

/**
 * 已连接的UDP套接字
 * 
 * 通过sendto函数传输数据的过程分为三步：
 *  1. 向 UDP 套接字注册目标IP和端口号
 *  2. 传输数据
 *  3. 删除 UDP 套接字中注册的目标地址信息
 * 
 * 因此如果要与主机进行长时间的通信，将 UDP 套接字进行连接设置会提供效率，将缩短
 * 第一个和第三个阶段所占用的时间，提高整体性能.
 * 
 * 配合 01_uecho_server 程序进行验证
 * 
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, net);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = udp_client_handle(argv[1], argv[2], &net);
    if (ret != 0)   handleError(getMsgByCode(ret));

    connect(net.sock, (struct sockaddr*)&net.addr, sizeof(net.addr));

    ssize_t rsize = 0;
    char buf[BUF_SIZE] = {0};
    while(1) {
        fputs("Inputs Message(Q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))  { break; }  
#if 0
        sendto(net.sock, buf, strlen(buf), 0, (struct sockaddr*)&net.addr, net.addr_len);

        clnt.addr_len = sizeof(clnt.addr);
        recvfrom(net.sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clnt.addr, &clnt.addr_len)
#else
        write(net.sock, buf, strlen(buf));
        rsize = read(net.sock, buf, sizeof(buf)-1);
#endif 
        buf[rsize] = 0;
        printf("Message clnt server: %s", buf);
    }
    close(net.sock);
    return 0;
}