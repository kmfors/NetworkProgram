#include "util_all.h"

/**
 * #include <sys/socket.h>
 * ssize_t send(int sockfd, const void *buf, size_t nbytes, int flags);
 * ssize_t recv(int sockfd, void *buf, size_t nbytes, int flags); 
 * 
 * 注意以上这两个函数的 flag 字段，它允许我们指定一些特殊操作：
 *  - MSG_OOB：用于传输带外数据（Out-of-Band），即紧急消息。这允许数据接收方得到紧急通知并优先处理
 * 
 * 通过MSG_OOB可选项传递数据时不会加快数据传输速度。真正意义上的OOB需要通过单独的通信路径高速传输数据
 * 但TCP不另外提供，只能是利用TCP的紧急模式（Urgent mode）进行传输.
 * 
 * MSG_OOB的真正意义是督促数据接收对象尽快处理数据
*/

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if(ret != 0)    handleError(getMsgByCode(ret));

    write(serv.sock, "123", strlen("123"));
    send(serv.sock, "4", strlen("4"), MSG_OOB);
    
    write(serv.sock, "567", strlen("567"));
    send(serv.sock, "890", strlen("890"), MSG_OOB);

    close(serv.sock);
    return 0;
}