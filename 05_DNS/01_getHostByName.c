#include "util_all.h"
#include <unistd.h>
#include <netdb.h>

/**
 * DNS 是对 IP 地址和域名进行相互转换的系统，其核心是 DNS 服务器。
 * 
 * DNS 是一种层次化管理的一种分布式数据库系统。当它收到自己无法解析的请求时，会向
 * 上级 DNS 服务器询问。当它询问成功后，会向下级DNS传递解析请求，得到IP地址后原路返回。
 * 
 * 利用域名获取IP地址 
 * struct hostent* gethostbyname(const char* hostname);
 * 
 * a.out www.baidu.com
*/

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <addr>\n", argv[0]);
        exit(1);
    }

    struct hostent* host = gethostbyname(argv[1]);
    if (!host)  handleError(getMsgByCode(ERR_GET_HOST_BY_NAME));

    printf("Official name: %s \n", host->h_name);
    
    for (int i = 0; host->h_aliases[i]; i++) {
        printf("Aliases %d: %s \n", i+1, host->h_aliases[i]);
    }

    printf("Address type: %s \n", (host->h_addrtype == AF_INET)? "AF_INET" : "AF_INET6");

    for (int i = 0; host->h_addr_list[i]; i++) {
        // h_addr_list兼容ipv6，struct in6_addr，因此h_addr_list是一个char*数组
        printf("IP addr %d: %s \n", i+1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
    
    return 0;
}
