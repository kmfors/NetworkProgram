#include "util_tool.h"
#include <arpa/inet.h>

/**
 * 将十进制格式的IP地址字符串，转换为网络字节序，并保存结果于指定的结构体中
 * 
 * int inet_aton(const char* string, struct in_addr* addr);
 * 成功返回1（true），失败返回0（false）
 * 
 * 特点：传递 in_addr 结构体后，自动将结果填入赋值
 * 
 * ======================================================
 * 将网络字节序整数型IP地址，转换为熟悉的字符串形式
 * char* inet_ntoa(struct in_addr addr);
 * 成功返回转换的字符串地址值，失败时返回-1
 * 注意：调用完该函数后，应立即保存该字符串信息，否则会被下一次的调用所覆盖
*/

int main(int argc, char* argv[]) 
{
    char* addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    int ret = inet_aton(addr, &addr_inet.sin_addr);
    if (ret == 0) {
        printf("Conversion error");
    } else {
        printf("Network ordered integer addr: %#x \n",
            addr_inet.sin_addr.s_addr);
    }
    return 0;
}