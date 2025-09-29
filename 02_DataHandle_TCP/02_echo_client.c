#include "../util_tool.h"
#include "../util_error.h"
#include "../util_sock.h"
#include <unistd.h>

/**
 * 迭代回声客户端
 * 
 * write 函数（发送）：从指定的地址进行，字节串的写入并发送
 *  - 消息区（用户态）--> 发送缓冲区（内核态）
 *  - 注意，字节串大小不固定，通常最后一位是 \0，所以取的是 length
 *  - write只是把数据交给内核，并不直接发送到网络
 * 
 * read 函数（接收）：从指定的地址进行，字节串的读取并接收
 *  - 接收缓冲区（内核态）--> 消息区（用户态）
 *  - 注意，读取的消息最后一位以 \0 结尾，所以取的是 sizeof-1
 *  - read只读取当前可用的数据，不保证是一次完整的应用层消息。
 * 
 * 迭代回声客户端缺点：
 *  - 服务器可能分多次发送数据，但客户端只读取一次
 *  - 大数据量时，一次read可能只读取部分数据
 *  - 剩余数据会留在内核缓冲区，导致后续读取的逻辑混乱
 * 虽然可以等待服务端全部发送结束后，再调用 read 函数，不过等多久是个问题
 * 
 * 正确的做法：设计应用层协议，让客户端知道何时算"接收完成"，而不是依赖单次read调用。
*/

#define BUF_SIZE 12

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
    
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_client_handle(argv[1], argv[2], &clnt);
    if (ret != 0) {
        handleError(getMsgByCode(ret));
    }
    printf("Connect Success! \n");
    
    ssize_t size = 0;
    char buf[BUF_SIZE] = {0};
    while(1) {
        fputs("Inputs msg(Q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))  { break; }
        
        write(clnt.socket, buf, strlen(buf));
        size = read(clnt.socket, buf, BUF_SIZE - 1);
        //size = read(clnt.socket, buf, sizeof(buf) - 1);

        buf[size] = 0; // 字符串以 \0 结尾
        printf("msg from server: %s", buf);
    }
    close(clnt.socket);
    return 0;
}


