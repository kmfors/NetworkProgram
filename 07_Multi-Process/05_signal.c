#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("Time out!");
    }
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT) {
        puts("CTRL+C pressed");
    }
}

int main(int argc, char* argv[]) {
    signal(SIGALRM, timeout);   // 注册信号 SIGALRM
    signal(SIGINT, keycontrol); // 注册信号 SIGINT

    alarm(2); // 设置定时2s

    for(int i = 0; i < 3; i++) {
        puts("wait..."); // 自带换行
        sleep(100);
    }
    return 0;
}

/*
*   按理来说，应总计耗时300s即5分钟，可是执行的结果却不到10s就结束了
*   原因是：发生信号时，将唤醒由于调用sleep函数而进入阻塞状态的进程。因此整个过程不超过10s
*/