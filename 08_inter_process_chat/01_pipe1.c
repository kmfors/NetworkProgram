#include <stdio.h>
#include <unistd.h>

/**
 * 通过管道实现进程间的通信
 * 
 * 成功返回0，失败返回-1
 * int pipe(int fileds[2]); 
 *  - fileds[0]：通过管道接收数据的句柄，管道出口
 *  - fileds[1]：通过管道发送数据的句柄，管道入口
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    int fds[2];
    char str[] = "who are you?";
    char buf[BUF_SIZE];


    pipe(fds); // 传递一个int数组
    pid_t pid = fork();

    if (pid == 0) {
        write(fds[1], str, sizeof(str));
    } else {
        read(fds[0], buf, BUF_SIZE);
        puts(buf);
    }
    return 0;
}