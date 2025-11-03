#include <sys/uio.h>
#include <stdio.h>

/**
 * 通过 writev 函数可以将分散保存在多个缓冲区中的数据一并发送，通过readv函数由多个缓冲接收
 * 适当使用这两个函数可以减少系统调用次数，提升I/O效率
 * 
 * #include <sys/uio.h>
 * ssize_t writev(int fileds, const struct iovec* iov, int iovcnt);
 * 成功返回发送的字节数，失败返回-1
 * 
*/

#define BUF_SIZE 100

int main(int argc, char* argv[])
{
    struct iovec vec[2];
    char buf1[BUF_SIZE] = {0,};
    char buf2[BUF_SIZE] = {0,};

    int str_len = 0;
    vec[0].iov_base = buf1;
    vec[0].iov_len = 5;
    vec[1].iov_base = buf2;
    vec[1].iov_len = BUF_SIZE;

    str_len = readv(0, vec, 2); // 从控制台读取
    printf("Read bytes: %d \n", str_len);
    printf("First message: %s \n", buf1);
    printf("Second message: %s \n", buf2);

    return 0;
}