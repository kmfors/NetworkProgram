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

int main(int argc, char* argv[])
{
    struct iovec vec[2];
    char buf1[] = "ABCDEFG";
    char buf2[] = "1234567";

    int str_len = 0;
    vec[0].iov_base = buf1;
    vec[0].iov_len = 3;
    vec[1].iov_base = buf2;
    vec[1].iov_len = 4;

    // 指定 vec 数组中的元素个数为2
    str_len = writev(1, vec, 2);
    puts("");
    printf("Write bytes: %d \n", str_len);
    return 0;
}