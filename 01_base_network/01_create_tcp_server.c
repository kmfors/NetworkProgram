#include "util_tool.h"
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

/**
 * 文件说明：创建TCP服务端
 * 
 * 创建 tcp 类型套接字的 socket 函数参数：
 *  - 第一个参数：PF_INET: IPV4 协议族
 *  - 第二个参数：SOCK_STREAM: 面向字节流的套接字类型
 *  - 第三个参数：IPPROTO_TCP：传输控制协议
 * 注意：前两个参数即可确定唯一具体协议，因为可省略第三参数的填写
 * 
 * ==========================================================
 * 创建 sockaddr_in 套接字地址结构体的成员说明：
 *  - sin_family：地址族
 *      -【AF_INET】 IPV4地址族
 *  - sin_addr：32 位IP地址 4字节
 *  - sin_port：16 位端口号 2字节
 * 注意：IP地址与端口号都要从主机序转换为网络序
 * 
 * =========================================================
 * 字节序：
 *  - 主机序：内存中存储数据的顺序
 *  - 网络序：网络传输的顺序
 * 
 * htonl函数：主->网 32位长整型 IP地址转换，【INADDR_ANY】任意IP地址
 * htons函数：主->网 16位短整型 端口号转换
 * 
 *
*/

int main(int argc, char* argv[]) 
{
    ASSERT_ARGC_SERVER(argc);

    // 1、创建指定传输类型的套接字
    int serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)  handleError("socket() error");

    // 2、创建套接字地址结构体，赋值IP地址与端口号
    INIT_STRUCT_FIELD(struct sockaddr_in, serv_addr);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 3、将套接字与地址信息做绑定
    int ret = bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if (ret == -1)  handleError("bind() error");

    // 4、监听套接字，及监听的最大上限
    ret = listen(serv_sock, 5);
    if (ret == -1)  handleError("listen() error");

    INIT_STRUCT_FIELD(struct sockaddr_in, clnt_addr);
    socklen_t clnt_addr_len = sizeof(clnt_addr);

    // 5、接收对方连接，并保存对方的套接字与地址信息
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
    if (clnt_sock == -1)  handleError("accept() error");

    char msg[] = "hello,world!";
    write(clnt_sock, msg, sizeof(msg));
    close(clnt_sock);
    close(serv_sock);
    
    return 0;
}
