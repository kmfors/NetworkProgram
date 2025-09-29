#include "../util_tool.h"
#include <fcntl.h>
#include <unistd.h>

/**
 * 简单的打开文件，写入文件
*/


int main(void) 
{
    char buf[] = "Let's go!\n";

    int fd = open("./WFile.txt", O_CREAT|O_WRONLY|O_TRUNC);
    if (fd == -1) {
        handleError("open() error!");
    }
    printf("file descriptor: %d \n", fd);

    ssize_t size = write(fd, buf, sizeof(buf));
    if (size == -1) {
        handleError("write() error!");
    }
    printf("write file size:%zd, data:%s", size, buf);

    close(fd);
    return EXIT_SUCCESS;
}
