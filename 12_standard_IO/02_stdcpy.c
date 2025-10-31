#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <errno.h>

/**
 * 标准IO函数的拷贝差异
 * 
 * 明显标准IO函数的速度比系统调用要快，可是也有缺点：
 *  - 不容易进行双向通信
 *  - 有时可能频繁调用 fflush 函数
 *  - 需要以FILE结构体指针形式返回文件描述符
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

    FILE* fp1 = fopen(src_file, "r");
    if (fp1 == NULL) { printf("Failed to open %s \n", src_file); return 1; }

    FILE* fp2 = fopen(dest_file, "w");
    if (fp2 == NULL) { printf("Failed to open %s \n", dest_file); fclose(fp1); return 1; }

    struct timeval start, end;
    gettimeofday(&start, NULL); // 开始计时

    char buf[BUF_SIZE] = {0};
    while(fgets(buf, BUF_SIZE, fp1) != NULL) {
        fputs(buf, fp2);
    }

    gettimeofday(&end, NULL); // 结束计时
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) * 1e-6;

    printf("花费时间: %.6f 秒\n", elapsed);
    printf("平均速度: %.2f MB/s\n", (total_bytes / (1024 * 1024)) / elapsed);

    fclose(fp1);
    fclose(fp2);
    return 0;
}