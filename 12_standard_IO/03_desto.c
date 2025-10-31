#define _POSIX_C_SOURCE 200809L // 含 C11 特性
#include <stdio.h>
#include <fcntl.h>

/**
 * 成功时返回转换的FILE结构体，失败时返回NULL
 * FILE* fdopen(int files, coonst char* mode); 
 * 
 * 纯系统调用方式：底层、高效，但缺少缓冲，每次write都是系统调用
 * 
 * 纯标准I/O方式：有缓冲区，减少系统调用，但无法精细控制打开属性
 * 
 * 混合方式的应用场景：
 *  - 需要特殊文件打开属性
 *  - 需要文件锁定
 *  - 套接字编程
 *  - 管道编程
 * 
*/

int main(void) 
{
    char* file = "data";

    // 系统调用设置的特殊属性
    int fd = open(file, O_WRONLY|O_CREAT|O_TRUNC, 0644); // 0644文件权限属性
    if (fd == -1)  { fputs("file open error", stdout); return -1; }

    // 转换为标准IO，获得缓冲优势
    FILE* fp = fdopen(fd, "w");
    fputs("Network C programming \n", fp);

    fclose(fp);
    return 0;
}