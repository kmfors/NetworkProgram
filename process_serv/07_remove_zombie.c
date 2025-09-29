#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void read_childproc(int sig) {
    int status = 0;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Remove proc id: %d \n", id);
        printf("Child send: %d \n", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) {
    pid_t pid;
    struct sigaction act;
    act.sa_handler  = read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0); // 子进程终止，则调用信号处理函数

    pid = fork();
    if (pid == 0) {
        puts("Child Process: Hi! I'm child process");
        sleep(2);
        return 12;
    } else {
        printf("Parent Process: Child proc id = %d \n", pid);
        pid = fork();
        if (pid == 0) {
            puts("Child Process: Hi! I'm child process");
            sleep(2);
            exit(24);
        } else {
            printf("Parent Process: Child proc id = %d \n", pid);
            // 等待发生SIGCHLD信号，使父进程暂停5次
            for (int i = 0; i < 5; i++) {
                puts("wait...");
                sleep(5);
            } // 发生信号时，父进程将被唤醒，因此实际暂停实际不到25s
        }
    }
}

/*
*   SIGCHLD 信号是在子进程结束时，由操作系统发送给父进程的信号。因此，read_childproc 函数只会在父进程中被调用。
*/