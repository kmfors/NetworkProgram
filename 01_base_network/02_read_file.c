#include "util_tool.h"
#include <fcntl.h>
#include <unistd.h>

/**
 * 简单的打开文件，读取文件
*/

int main(void) 
{
    int fd = open("./RFile.txt", O_RDONLY);
    if (fd == -1) {
        handleError("open() error!");
    } 
    printf("file descriptor: %d \n", fd);

    char buf[30];
    ssize_t size = read(fd, buf, sizeof(buf));
    if (size == -1) {
        handleError("read() error!");
    }
    printf("read file size:%d, data:%s", size, buf);
    
    close(fd);
    return EXIT_SUCCESS;
}
