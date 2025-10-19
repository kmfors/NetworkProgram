#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * 销毁僵尸进程：成功返回终止的子进程ID或0，失败返回-1
 * pid_t waitpid(pid_t pid, int* statloc, int options); 
 *  - pid：指定等待的子进程ID；若传递-1，则与wait函数相同，等待任意子进程终止
 *  - options：
 *    - 0：阻塞等待（默认）
 *    - WNOHANG：非阻塞，立即返回
 * 
 * 
 * 宏调用
 * WIFEXITED：子进程正常终止时返回 true
 * WEXITSTATUS：返回子进程的返回值 
 * 
 * WTERMSIG：提取导致子进程终止的信号编号
 * WIFSIGNALED：判断子进程是否被信号（signal）终止
 * 
 * 可是如果设计的waitpid函数调用是非阻塞+循环，确实和阻塞等待差不多
 * 但父进程在等待子进程的同时还能做其他工作
 * 
*/

int main(int argc, char* argv[]) {
    int status = 0;
    pid_t pid = fork();

    if (pid == 0) { sleep(15); return 24; }

    pid_t result; // 非阻塞+循环
    while ((result = waitpid(-1, &status, WNOHANG)) == 0) {
        printf("Child still running, waiting...\n");
        sleep(3);
    }
    if (result > 0 && WIFEXITED(status)) {
        printf("Child PID %d exited normally with status: %d\n", 
            result, WEXITSTATUS(status));
    }

    return 0;
}