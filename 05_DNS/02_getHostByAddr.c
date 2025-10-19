#include "util_all.h"
#include <unistd.h>
#include <netdb.h>

/**
 * 通过 IP 地址获取域名
 * struct hostent* gethostbyaddr(const char* addr, socklen_t len, int family);
 * 
 */

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <IP>\n", argv[0]);
        exit(1);
    }
    INIT_STRUCT_FIELD(struct sockaddr_in, addr);
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    
    struct hostent* host = gethostbyaddr((char*)&addr.sin_addr, 4, AF_INET);
    if (!host)  handleError(getMsgByCode(ERR_GET_HOST_BY_ADDR));

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