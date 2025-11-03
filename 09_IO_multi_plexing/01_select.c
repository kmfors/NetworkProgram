#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

/**
 * select 函数时最具有代表性的实现复用服务器端的方法
 * 
 * 如果设置监听文件描述符的位数为1，则表示该文件描述符是监视对象
 * 
 * FD_ZERO(fd_set* fdset)：将fd_set变量的所有位初始化为0
 * FD_SET(int fd, fd_set* fdset)：注册文件描述符信息
 * FD_CLR(int fd, fd_set* fdset)：清除文件描述符信息
 * FD_ISSET(int fd, fd_set* fdset)：若包含此文件描述符，则返回true
 * 
 * int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);
 * 参数说明：
 *  - nfds: 指定被监听的文件描述符的最大值加1。即，所有被监视的文件描述符的范围是从0到nfds-1。
 *  - readfds: 将想要监视是否可读的文件描述符注册到监听集合里。如果不需要监视可读性，可以传入NULL。
 *  - writefds: 将想要监视是否可写的文件描述符注册到监听集合里。如果不需要监视可写性，可以传入NULL。
 *  - exceptfds: 将想要监视是否发生异常的文件描述符注册到监听集合里。如果不需要监视异常，可以传入NULL。
 *  - timeout: 设置超时时间。如果设为NULL，则无限期地阻塞，直到有文件描述符就绪。如果设为0，则会立即返回，轮询一次后不管有没有就绪都返回。
 * 返回值：
 *  - 成功时，返回就绪的文件描述符的总数（包括所有集合）。如果超时，返回0。
 *  - 错误时，返回-1，并设置相应的errno。
 * 
 * select函数调用完后，向其传递的fd_set变量中将发生变化，原来为1的所有位均变为0 
 * select函数只有在监视的文件描述符发生变化时才返回，如果未发生变化，就会进入到阻塞状态
 * 而指定超时时间就是为了防止这种阻塞情况的发生
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    int file_desc = 0;  // 设置监听的文件描述符为：标准输入
    char buf[BUF_SIZE];
    struct timeval timeout;

    fd_set reads, temps;
    FD_ZERO(&reads);
    FD_SET(file_desc, &reads);
    
    //timeout.tv_sec = 5; // 秒
    //timeout.tv_usec = 5000; // 毫秒

    while(1) {
        temps = reads; // 复制原始集合到临时变量
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        int result = select(file_desc+1, &temps, 0, 0, &timeout);
        if (result == -1) {
            puts("select() error!");
            break;
        } else if (result == 0) {
            puts("Time-out!");
        } else {
            if (FD_ISSET(file_desc, &temps)) {
                int str_len = read(file_desc, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console: %s", buf);
            }
        }
    }
    return 0;
}