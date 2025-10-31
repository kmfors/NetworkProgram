#define _POSIX_C_SOURCE 200809L // 含 C11 特性
#include <stdio.h>
#include <fcntl.h>

/**
 * 成功时返回转换后的文件描述符，失败时返回-1
 * int fileno(FILE* stream);
 * 
 * 
*/

int main(void) 
{
    char* file = "data";

    // 系统调用设置的特殊属性
    int fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0644); // 0644文件权限属性
    if (fd == -1)  { fputs("file open error", stdout); return -1; }

    printf("First file descriptor: %d \n", fd);

    FILE* fp = fdopen(fd, "w");
    fputs("TCP/IP SOCKET PROGRAMMING \n", fp);
    printf("Second file descriptor: %d \n", fileno(fp));

    fclose(fp);
    return 0;
}