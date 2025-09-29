#ifndef _UTIL_SOCK_H_
#define _UTIL_SOCK_H_

// 网络编程包装头文件

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#ifndef INIT_STRUCT_FIELD
#define INIT_STRUCT_FIELD(Type, field) \
    Type field;                 \
    memset(&field, 0, sizeof(field));
#endif

typedef struct sock_info
{
    int socket;
    socklen_t addr_len;
    struct sockaddr_in addr;
} sock_info_t;

typedef sock_info_t serv_sock_info_t;
typedef sock_info_t clnt_sock_info_t;

const int g_listen_max = 5;
static inline int 
tcp_server_listen(const char* port, serv_sock_info_t* serv, clnt_sock_info_t* clnt) 
{
    // 1、创建指定传输类型的套接字
    serv->socket = socket(PF_INET, SOCK_STREAM, 0);
    if (serv->socket == -1)  return 1001;

    serv->addr.sin_family = AF_INET;
    serv->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv->addr.sin_port = htons(atoi(port));

    // 3、将套接字与地址信息做绑定
    int ret = bind(serv->socket, (struct sockaddr*)&serv->addr, sizeof(serv->addr));
    if (ret == -1)  return 1002;

    // 4、监听套接字，及监听的最大上限
    ret = listen(serv->socket, g_listen_max);
    if (ret == -1)  return 1003;

    clnt->addr_len = sizeof(clnt->addr);
    return 0;
}

// 5、接收对方连接，并保存对方的套接字与地址信息
#define TCP_SERVER_ACCEPT(serv, clnt)\
    clnt->socket = accept(serv->socket, (struct sockaddr*)&clnt->addr, &clnt->addr_len);\
    if (clnt->socket == -1)  return 1004;\

static inline int 
tcp_server_accept(serv_sock_info_t* serv, clnt_sock_info_t* clnt) 
{
    clnt->socket = accept(serv->socket, (struct sockaddr*)&clnt->addr, &clnt->addr_len);
    if (clnt->socket == -1)  return 1004;
}


// TCP 服务端流程
static inline int 
tcp_server_handle(const char* port, serv_sock_info_t* serv, clnt_sock_info_t* clnt) 
{
    int ret = tcp_server_listen(port, serv, clnt);
    if (ret != 0)   return ret;

    ret = tcp_server_accept(serv, clnt);
    if (ret != 0)   return ret;
    
    return 0;
}

// TCP 客户端流程
static inline int 
tcp_client_handle(const char* ip, const char* port, clnt_sock_info_t* clnt)
{
    // 1、创建指定传输类型的套接字
    clnt->socket = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt->socket == -1)  return 1001;

    // 2、创建套接字地址结构体，赋值IP地址与端口号
    clnt->addr.sin_family = AF_INET;
    clnt->addr.sin_addr.s_addr = inet_addr(ip);
    clnt->addr.sin_port = htons(atoi(port));

    clnt->addr_len = sizeof(clnt->addr);
    // 3、将套接字与地址信息做连接处理
    int ret = connect(clnt->socket, (struct sockaddr*)&clnt->addr, clnt->addr_len);
    if (ret == -1)  return 1005;

    return 0;
}

// TCP 服务端流程
static inline int 
udp_server_handle(const char* port, serv_sock_info_t* serv) {
    // 1、创建指定传输类型的套接字
    serv->socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (serv->socket == -1)  return 1001;

    serv->addr.sin_family = AF_INET;
    serv->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv->addr.sin_port = htons(atoi(port));

    // 3、将套接字与地址信息做绑定
    int ret = bind(serv->socket, (struct sockaddr*)&serv->addr, sizeof(serv->addr));
    if (ret == -1)  return 1002;
    return 0;
}


/**
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

#endif // ! _UTIL_SOCK_H_