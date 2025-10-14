#include "util_all.h"
#include <unistd.h>

/**
 * 存在数据边界的UDP套接字
 * 
 * UDP 是具有数据边界的协议，传输中调用 IO 函数的次数非常重要，因此输入函数的调用次数应和
 * 输出函数的调用次数完全一致，才能够保证接收全部已发送的数据。示例验证：
 * 
 * UDP 客户端调用3次sendto函数后结束，此时数据已经传输到了服务端。如果是TCP服务端的话，只需要
 * 调用一次read函数即可读取数据。而UDP服务端不同，必须也调用3次的recvfrom函数
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = udp_server_handle(argv[1], &serv);
    if (ret != 0)   handleError(getMsgByCode(ret));

    char buf[BUF_SIZE] = {0};
    for (int i = 0; i < 3; i++) {
        sleep(5);
        clnt.addr_len = sizeof(clnt.addr);
        recvfrom(serv.sock, buf, BUF_SIZE, 0, (struct sockaddr*)&clnt.addr, &clnt.addr_len);

        printf("Message %d: %s \n", i+1, buf);
    }
    close(serv.sock);
    return 0;
} 