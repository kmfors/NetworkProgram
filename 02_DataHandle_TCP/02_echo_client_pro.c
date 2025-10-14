#include "util_all.h"
#include <unistd.h>

/**
 * 迭代回声客户端 - 升级版(提前确定接收数据的大小)
 * 
 * 迭代回声客户端缺点：
 *  - 服务器可能分多次发送数据，但客户端只读取一次
 *  - 大数据量时，一次read可能只读取部分数据
 *  - 剩余数据会留在内核缓冲区，导致后续读取的逻辑混乱
 * 虽然可以等待服务端全部发送结束后，再调用 read 函数，不过等多久是个问题
 * TCP套接字的数据收发无边界！
 * 
 * 暂时的解决方法：提前确定接收数据的大小，不足的，循环调用read函数
 * 
 * 但我们应该意识到，这不是长久之计，很多情况下我们不可能预知接收数据的长度，
 * 更不知如何收发数据，因此需要的是应用层协议的定义。
 * 
 * 
*/
#define BUF_SIZE 10
int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
    
    INIT_STRUCT_FIELD(clnt_sock_info_t, serv);

    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if (ret != 0) {
        handleError(getMsgByCode(ret));
    }
    printf("Connect Success! \n");
    
    ssize_t wsize, total_rsize = 0;
    char buf[BUF_SIZE] = {0};
    while(1) {
        fputs("Inputs msg(Q to quit): ", stdout);
        fgets(buf, BUF_SIZE, stdin);
        if(!strcmp(buf, "q\n") || !strcmp(buf, "Q\n"))  { break; }

        wsize = write(serv.sock, buf, strlen(buf));
        memset(buf, 0, sizeof(buf));

        ssize_t rsize = 0;
        while (total_rsize < wsize) { // 优化点！
            //rsize = read(serv.sock, buf, 2); // 2个字节的捞取
            rsize = read(serv.sock, &buf[total_rsize], BUF_SIZE - 1); 
            if (rsize == -1)  handleError(getMsgByCode(1006));
            total_rsize += rsize; 
        }
        buf[total_rsize] = 0; // 字符串以 \0 结尾
        printf("buf from server: %s", buf);
    }
    close(serv.sock);
    return 0;
}

/** 
* 改进：
* 1. 记录发送数据的长度
* 2. 每次判断接收总和数据的长度是否达到原始发送数据的长度
*    是：一次完整读出
*    否：多次读取数据，计算接收返回数据长度的总和
* 
* 缺点1：回声客户端可以提前知道接收数据的长度，但这属于理想情况。若无法预知接收数据长度，又该如何收发数据呢？
* 答：需要应用层协议的定义。
* 缺点2：只有接收完服务端的回声数据后，才能传输下一批数据
* 之前的回声服务器/客户端就定义了协议“收到Q就立即终止连接”。同样，收发数据过程中也需要定义好规则（协议）以表示数据的边界，
* 或提前告知收发数据的大小。服务器/客户端实现过程中逐步定义的这些规则集合就是应用层协议。
*/
