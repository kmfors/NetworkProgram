#include "../util_tool.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * 文件说明：创建TCP客户端
 * 注意：TCP套接字传输的数据不存在数据边界的，怎么验证？
 * 在客户端中分次调用read函数以接收服务端发送的全部数据
 * 
*/

int main(int argc, char* argv[])
{
    ASSERT_ARGC_CLIENT(argc);
 
    // 1、创建指定传输类型的套接字
    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)  handleError("socket() error");

    // 2、创建套接字地址结构体，赋值IP地址与端口号
    INIT_STRUCT_FIELD(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // 3、将套接字与地址信息做连接处理
    int ret = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)  handleError("connect() error!");
    
    char msg[30];
    int msg_len = read(sock, msg, sizeof(msg)-1);
    if (msg_len == -1)  handleError("read() error!");

    printf("msg from server: %s \n", msg);
    close(sock);
    return 0;
}

