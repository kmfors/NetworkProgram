#define _POSIX_C_SOURCE 200809L // 含 C11 特性
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

/**
 * SIGCHLD 信号是在子进程结束时，由操作系统发送给父进程的信号。因此，read_childproc 函数只会在父进程中被调用。
 * 
 * 注意信号处理是异步的
 *  - 非阻塞：父进程的主要工作流不会被阻塞
 *  - 中断：当信号到达时，父进程的当前执行会暂时中断，跳转到信号处理函数
 *  - 恢复：信号处理完成后，父进程恢复到原来的执行点继续工作
 * 
 * 
*/

void read_childproc(int sig) {
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Remove proc id: %d, it's return:%d \n", id, WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) {

    struct sigaction act;
    act.sa_handler  = read_childproc;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaction(SIGCHLD, &act, 0); // 注册子进程终止信号

    pid_t pid = fork();
    if (pid == 0) {
        printf("Child Process 1: PID=%d, exit_code: 12\n", getpid());
        sleep(6);
        return 12;
    }
    printf("Parent Process: Created child 1 with PID=%d\n", pid);

    pid = fork();
    if (pid == 0) {
        printf("Child Process 2: PID=%d, exit_code: 24\n", getpid());
        sleep(6);
        exit(24);
    } 
    printf("Parent Process: Created child 2 with PID=%d\n", pid);

    for (int i = 0; i < 5; i++) {
        puts("wait.....");
        sleep(5);
    }
    printf("Done!\n");
    return 0;
}


// printf("\r\033[K");  // \033[K 清除从光标到行尾的内容