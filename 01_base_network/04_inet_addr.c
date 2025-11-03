#include <stdio.h>
#include <arpa/inet.h>
/**
 * 将十进制格式的IP地址字符串，转换为32位整数型
 * 
 * in_addr_t inet_addr(const char* string);
 * 成功返回32位大端序整数型，失败返回 INADDR_NONE（检测无效IP地址）
 * 
 * 特点：需要将转换后的IP地址，再赋值给 in_addr 结构体变量中
 * 
*/

int main(int argc, char* argv[]) 
{
    char* addr1 = "1.2.3.4";
    char* addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE) 
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE) 
        printf("Error occured! \n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    return 0;
}