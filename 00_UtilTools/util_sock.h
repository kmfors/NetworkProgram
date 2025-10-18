#ifndef _UTIL_SOCK_H_
#define _UTIL_SOCK_H_

// 网络编程包装头文件

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

//============================= 宏定义 ==================================

#ifndef INIT_STRUCT_FIELD
#define INIT_STRUCT_FIELD(Type, field) \
    Type field;                 \
    memset(&field, 0, sizeof(field));
#endif



//=========================== 结构体定义 ==================================

typedef struct sock_info
{
    int sock;
    socklen_t addr_len;
    struct sockaddr_in addr;
} sock_info_t;

typedef sock_info_t serv_sock_info_t;
typedef sock_info_t clnt_sock_info_t;



//============================= TCP接口 =================================

// 网络连接监听
int tcp_listen_func(const char* port, serv_sock_info_t* serv, clnt_sock_info_t* clnt);

// 创建服务端
int tcp_server_handle(const char* port, serv_sock_info_t* serv, clnt_sock_info_t* clnt);

// 创建客户端
int tcp_client_handle(const char* ip, const char* port, serv_sock_info_t* serv);



//============================= TCP接口 =================================

// 创建服务端
int udp_server_handle(const char* port, serv_sock_info_t* serv);

// 创建客户端
int udp_client_handle(const char* ip, const char* port, serv_sock_info_t* serv);


#endif // ! _UTIL_SOCK_H_