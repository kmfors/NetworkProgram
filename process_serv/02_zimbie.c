#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    pid_t pid = fork();

    if (pid == 0) {
        puts("Hi, I'm a child process");
    } else {
        printf("Child Process ID: %d \n", pid);
        sleep(30);
    }

    if (pid == 0)
        // 由于父进程沉睡，子进程完成执行后成为僵尸进程
        puts("End child process \n"); 
    else
        puts("End parent process \n");
    return 0;
}

/*
*   经过30s的等待时间后，父进程与僵尸子进程同时销毁 ，可用 ps au 命令查看
*/