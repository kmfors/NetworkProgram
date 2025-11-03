#include <stdio.h>
#include <unistd.h>

/**
 * 虽然可以通过fdopen获取套接字对应的文件描述符，但是不建议使用，因为当使用多个dfopen对同一个套接字
 * 创建文件描述符只会是同一个，对某个文件指针进行fclose后，连同文件描述符与套接字一起停止工作，导致
 * 其他的文件指针不可用（即无法半关闭套接字）
 * 
 * 读模式（FILE指针） --
 *                文件描述符 <--> 套接字
 * 写模式（FILE指针） --
 *
 * ================== 使用dup函数 ======================
 * 读模式（FILE指针） -- 文件描述符（原件） <--> 套接字
 *                
 * 写模式（FILE指针） -- 文件描述符（副本） <--> 套接字
 *
 * 为了访问同一个文件或套接字，创建另一个文件描述符，即 dup函数与dup2函数
*/

int main(void)
{
    char str1[] = "Hi~ \n";
    char str2[] = "It's nice day~ \n";

    int cfd1 = dup(1); // 拷贝stdout文件描述符,分配新的文件描述符3
    int cfd2 = dup2(cfd1, 7); // 拷贝cfd1文件描述符,指定分配新的文件描述符7

    printf("fd1: %d, fd2: %d \n", cfd1, cfd2);

    write(cfd1, str1, sizeof(str1));
    write(cfd2, str2, sizeof(str2));

    close(cfd1);
    close(cfd2);

    write(1, str1, sizeof(str1));
    close(1);

    write(1, str2, sizeof(str2)); // 此时1关闭，所以无法写入
    return 0;
}