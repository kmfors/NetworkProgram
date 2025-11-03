#include <stdio.h>
#include <unistd.h>

/**
 * 通过管道进行进程间双向通信
 * 
 * 注意：数据进入管道之后便成为了无主数据，那么就看哪一个进程的read函数最先读取到
 * 所以，当子进程将数据写入管道后，如果取消子进程的睡眠操作，那么子进程将读到自己
 * 发送的数据；而父进程则调用read函数后，陷入无限等待中，等待新的数据进入管道。
 * 
 * 结论：仅凭 1 个管道进行双向通信很难，起码要创建2个管道进行通信
 *  
*/

#define BUF_SIZE 30

int main(int argc, char* argv[]) {
    int fds[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];

    pipe(fds);
    pid_t pid = fork();
    if (pid == 0) {
        write(fds[1], str1, sizeof(str1));
        sleep(2); // 注释看效果
        read(fds[0], buf, BUF_SIZE);
        printf("Child proc output: %s \n", buf);
    } else {
        read(fds[0], buf, BUF_SIZE);
        printf("Parent proc output: %s \n", buf);
        write(fds[1], str2, sizeof(str2));
        sleep(3);
    }
    return 0;
}

/*
* 若将第16行注释掉，再运行程序将是什么结果，并且原因是什么？
* 如果父进程的read函数快于子进程的read函数，会完整的打印内容，并发送内容；而子进程同样也会接收内容打印
* 如果子进程的read函数快于父进程的read函数，会导致父进程的read函数一直处于阻塞状态，因为父进程无数据可读
* 所以只用一个管道，进行双向通信并非易事。
*/