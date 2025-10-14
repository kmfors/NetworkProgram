#include "util_all.h"
#include <unistd.h>

/**
 * 存在数据边界的UDP套接字
 * 
 * UDP 是具有数据边界的协议，传输中调用 IO 函数的次数非常重要，因此输入函数的调用次数应和
 * 输出函数的调用次数完全一致，才能够保证接收全部已发送的数据。
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(clnt_sock_info_t, serv);

    int ret = udp_client_handle(argv[1], argv[2], &serv);
    if (ret != 0)   handleError(getMsgByCode(ret));

    // 未明确数组长度的，用sizeof，发送结尾的'\0'
    char msg1[] = "Hi!";
    char msg2[] = "I'm another UDP host!";
    char msg3[] = "Nice to meet you!";

    // 验证 UDP 的数据边界
    sendto(serv.sock, msg1, sizeof(msg1), 0, (struct sockaddr*)&serv.addr, sizeof(serv.addr));
    sendto(serv.sock, msg2, sizeof(msg2), 0, (struct sockaddr*)&serv.addr, sizeof(serv.addr));
    sendto(serv.sock, msg3, sizeof(msg3), 0, (struct sockaddr*)&serv.addr, sizeof(serv.addr));

    close(serv.sock);
    return 0;
}