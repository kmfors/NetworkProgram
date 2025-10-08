#include "util_sock.h"

const int g_listen_max = 5;

// 服务端绑定
int server_bind(const char* port, int sock_type, serv_sock_info_t* serv) {
    // 1、创建指定传输类型的套接字
    serv->sock = socket(PF_INET, sock_type, 0);
    if (serv->sock == -1)  return 1001;

    serv->addr.sin_family = AF_INET;
    serv->addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv->addr.sin_port = htons(atoi(port));

    // 3、将套接字与地址信息做绑定
    int ret = bind(serv->sock, (struct sockaddr*)&serv->addr, sizeof(serv->addr));
    if (ret == -1)  return 1002;
    return 0;
}
// 客户端连接
int client_connect(const char* ip, const char* port, int sock_type, clnt_sock_info_t* clnt)
{
    // 1、创建指定传输类型的套接字
    clnt->sock = socket(PF_INET, sock_type, 0);
    if (clnt->sock == -1)  return 1001;

    // 2、创建套接字地址结构体，赋值IP地址与端口号
    clnt->addr.sin_family = AF_INET;
    clnt->addr.sin_addr.s_addr = inet_addr(ip);
    clnt->addr.sin_port = htons(atoi(port));

    clnt->addr_len = sizeof(clnt->addr);

    // 3、将套接字与地址信息做连接处理（UDP不用发起连接）
    if (sock_type == SOCK_STREAM) {
        int ret = connect(clnt->sock, (struct sockaddr*)&clnt->addr, clnt->addr_len);
        if (ret == -1)  return 1005;
    }
    return 0;
}

// tcp 监听
int tcp_listen(serv_sock_info_t* serv, clnt_sock_info_t* clnt) 
{
    // 4、监听套接字，及监听的最大上限
    int ret = listen(serv->sock, g_listen_max);
    if (ret == -1)  return 1003;

    clnt->addr_len = sizeof(clnt->addr);
    return 0;
}

// tcp 接收连接
int tcp_accept(serv_sock_info_t* serv, clnt_sock_info_t* clnt) 
{
    // 5、接收对方连接，并保存对方的套接字与地址信息
    clnt->sock = accept(serv->sock, (struct sockaddr*)&clnt->addr, &clnt->addr_len);
    if (clnt->sock == -1)  return 1004;

    return 0;
}

//=================头文件的实现函数========================================================
// tcp的listen处理
int tcp_listen_func(const char* port, serv_sock_info_t* serv, clnt_sock_info_t* clnt) 
{
    int ret = server_bind(port, SOCK_STREAM, serv);
    if (ret != 0)   return ret;

    ret = tcp_listen(serv, clnt);
    if (ret != 0)   return ret;

    return 0;
}

// TCP 服务端流程
int tcp_server_handle(const char* port, serv_sock_info_t* serv, clnt_sock_info_t* clnt) 
{
    int ret = server_bind(port, SOCK_STREAM, serv);
    if (ret != 0)   return ret;

    ret = tcp_listen(serv, clnt);
    if (ret != 0)   return ret;

    ret = tcp_accept(serv, clnt);
    if (ret != 0)   return ret;
    
    return 0;
}

// tcp 客户端流程
int tcp_client_handle(const char* ip, const char* port, clnt_sock_info_t* clnt)
{
    int ret = client_connect(ip, port, SOCK_STREAM, clnt);
    if (ret != 0)   return ret;

    return 0;
}

//=========================================================================
// UDP 服务端流程
int udp_server_handle(const char* port, serv_sock_info_t* serv) {
    int ret = server_bind(port, SOCK_DGRAM, serv);
    if (ret != 0)   return ret;
    return 0;
}
// udp 客户端流程
int udp_client_handle(const char* ip, const char* port, clnt_sock_info_t* clnt)
{
    int ret = client_connect(ip, port, SOCK_DGRAM, clnt);
    if (ret != 0)   return ret;

    return 0;
}

// 打印IP地址信息
void print_addr(const sock_info_t* sock_info, const char *tag) {
    char ip_str[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &(sock_info->addr.sin_addr), ip_str, sizeof(ip_str));
    unsigned short port = ntohs(sock_info->addr.sin_port);
    
    if (tag && tag[0] != '\0') {
        printf("[%s] IP: %s, Port: %d\n", tag, ip_str, port);
    } else {
        printf("IP: %s, Port: %d\n", ip_str, port);
    }
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

