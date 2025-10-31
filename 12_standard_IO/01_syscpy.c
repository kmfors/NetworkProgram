#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * 系统调用的拷贝差异
*/

#define BUF_SIZE 128

int main(int argc, char* argv[])
{
    
    char* src_file = "compute_system.pdf";
    char* dest_file = "cpy.txt";

    struct stat st;
    double total_bytes = 0;
    if (stat(src_file, &st) == 0) {
        total_bytes = (double)st.st_size;  // 保持字节单位
        printf("源文件大小: %.2f MB\n", total_bytes / (1024 * 1024));
    }

    int fd1 = open(src_file, O_RDONLY);
    if (fd1 == -1) { printf("Failed to open %s \n", src_file); return 1; }

    int fd2 = open(dest_file, O_WRONLY|O_CREAT|O_TRUNC);
    if (fd2 == -1) { printf("Failed to open %s \n", dest_file); close(fd1); return 1; }

    struct timeval start, end;
    gettimeofday(&start, NULL); // 开始计时

    int len = 0;
    char buf[BUF_SIZE] = {0};
    while((len = read(fd1, buf, sizeof(buf))) > 0) {
        write(fd2, buf, len);
    }

    gettimeofday(&end, NULL); // 结束计时
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;

    printf("花费时间: %.6f 秒\n", elapsed);
    printf("平均速度: %.2f MB/s\n", (total_bytes / (1024 * 1024)) / elapsed);

    close(fd1);
    close(fd2);
    return 0;
}