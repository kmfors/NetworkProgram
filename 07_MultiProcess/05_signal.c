#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * 现在我们已经知道了进程的创建与销毁方法，但是子进程究竟何时终止，调用waitpid函数难道无休止等待吗？
 * 
 * 信号：在特定事件发生时，由操作系统向进程发送的消息 
 * void (*signal(int signo, void (*func)(int)))(int); 
 *  - SIGALRM：已到通过调用 alarm 函数注册的时间
 *  - SIGINT：输入 Ctrl+C
 *  - SIGCHLD：子进程终止
 * 
 * unsignal int alarm(unsigned int seconds); 返回0或以秒为单位的剩余时间
 * 
 * alarm(2) 在信号处理函数中被重复调用，形成了循环定时：
 *  - alarm(2) → 2秒后 → timeout && alarm(2) → 2秒后 → timeout && alarm(2) → ...
 * 
 * 为什么程序执行后，wait...与Time out交替打印呢？
 *  - 当进程在 sleep() 等阻塞调用中时，如果收到信号，系统会立即中断 sleep() 状态
 *  - 转去执行信号处理函数，
 *  - 信号处理完成后，sleep() 不会继续剩余时间，而是直接返回
 * 
*/

// 异步调用
void timeout(int sig) {
    if (sig == SIGALRM) puts("Time out!");
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT) puts("CTRL+C pressed");
}

int main(int argc, char* argv[]) 
{
    signal(SIGALRM, timeout);   // 注册信号 SIGALRM
    signal(SIGINT, keycontrol); // 注册信号 SIGINT

    alarm(2); // 设置定时2s

    // 打断次数设为3
    int break_count = 3;
    for(int i = 0; i < break_count; i++) {
        puts("wait..."); // 自带换行
        sleep(100);
    }
    return 0;
}