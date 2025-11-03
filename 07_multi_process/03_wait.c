#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * 销毁僵尸进程：成功返回终止的子进程ID，失败返回-1
 * pid_t wait(int* statloc); 
 * 
 * 宏调用
 * WIFEXITED：子进程正常终止时返回 true
 * WEXITSTATUS：返回子进程的返回值 
 * 
 * WTERMSIG：提取导致子进程终止的信号编号
 * WIFSIGNALED：判断子进程是否被信号（signal）终止
 * 
 * wait 系统调用会按照子进程终止的先后顺序，依次回收子进程并读取其退出状态信息。
 * 阻塞调用：如果没有子进程结束，会一直等待
*/

int main(int argc, char* argv[]) {
    int status;
    pid_t pid1, pid2, term_pid;

    pid1 = fork();
    if (pid1 == 0)   return 3; // 子进程结束
    printf("Child One PID: %d\n", pid1);

    pid2 = fork();
    if (pid2 == 0)   exit(7); // 子进程退出
    printf("Child Two PID: %d\n", pid2);
    
    // 之前终止的子进程相关信息将保存到status中
    while ((term_pid = wait(&status)) != -1) {
        if (WIFEXITED(status)) {
            printf("Child PID %d exited with status: %d\n", 
                   term_pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Child PID %d terminated by signal: %d\n", 
                   term_pid, WTERMSIG(status));
        }
    }

    sleep(30); 
    return 0;
}