#ifndef _UTIL_ALL_H_
#define _UTIL_ALL_H_

#ifndef _UTIL_TOOL_H_
#include "util_tool.h"
#endif

#ifndef _UTIL_SOCK_H_
#include "util_sock.h"
#endif

#ifndef _UTIL_ERROR_H_
#include "util_error.h"
#endif

// 打印IP地址信息
static inline void print_addr(const sock_info_t* sock_info, const char *tag) 
{
    char ip_str[INET_ADDRSTRLEN] = {0};
    inet_ntop(AF_INET, &(sock_info->addr.sin_addr), ip_str, sizeof(ip_str));
    unsigned short port = ntohs(sock_info->addr.sin_port);
    
    if (tag && tag[0] != '\0') {
        printf("[%s] IP: %s, Port: %d\n", tag, ip_str, port);
    } else {
        printf("IP: %s, Port: %d\n", ip_str, port);
    }
}

#endif