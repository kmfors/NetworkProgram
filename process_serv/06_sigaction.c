#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("Time out!");
    }
    alarm(2);
}


int main(int argc, char* argv[]) {

    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;

    sigaction(SIGALRM, &act, 0); // 注册信号 SIGALRM

    alarm(2); // 设置定时2s

    for(int i = 0; i < 3; i++) {
        puts("wait..."); // 自带换行
        sleep(100);
    }
    return 0;
}

/*
*  sigaction函数完全可以替换 signal函数的功能，但在这里只体现所替代的功能展示。
*/