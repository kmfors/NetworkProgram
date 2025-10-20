#define _POSIX_C_SOURCE 200809L // 含 C11 特性
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

/**
 * sigaction 比 signal 更稳定和可靠,是因为它将所有与信号处理相关的不确定、隐式行为都变成了由程序员显式控制的、确定的行为
 * 
 * 核心配置器，设置信号处理方式
 * int sigaction(int signo, const struct sigaction* act, struct sigaction* oldact);
 *  - signo：传递信号信息
 *  - act：注册的信号处理函数指针
 *  - oldact：之前注册的信号处理函数指针，若不需要传递0
 *  - 成功返回0，失败返回-1
 * 
 * sigset_t 表示一个信号集合
 * struct sigaction 主要包括三部分：
 *  - sa_handler：回调函数 void(int)
 *  - sa_mask：信号屏蔽字!  类型为 sigset_t
 *  - sa_flags：修改行为的标志位
 * 
 * 信号集操作函数群：
 *  - sigemptyset - 清空信号集（使用信号集前，必须调用）
 *  - sigfillset - 填充所有信号
 *  - sigaddset - 添加信号到集合
 *  - sigdelset - 从集合删除信号
 *  - sigismember - 检查信号是否在集合中
 * 
 * 待学习：
 *  - sigprocmask() - 进程信号屏蔽
 *  - sigpending() - 检查待处理信号
 *  - sigsuspend() - 原子性的等待
 * 
 * 补充：pause() 使当前进程挂起（睡眠），直到收到任何信号为止。
 *  - 通常返回 -1，并设置 errno 为 EINTR（表示被信号中断）
 *  - 只有在信号处理函数执行完毕后才返回
 * 
*/

void timeout(int sig) {
    if (sig == SIGALRM) {
        puts("Time out!");
        alarm(10);
    } else if (sig == SIGINT) {
        puts("Ctrl+C pressed!");
    }
}

int main(int argc, char* argv[]) 
{
    struct sigaction act;
    act.sa_handler = timeout;
    act.sa_flags = 0;
    
    // 清空并设置屏蔽字
    sigemptyset(&act.sa_mask);
    // sigaddset(&act.sa_mask, SIGQUIT); // 添加其他信号到屏蔽集
    // sigdelset(&act.sa_mask, SIGQUIT); // 从屏蔽集删除其他信号
    
    // 对行为事件注册多个信号
    sigaction(SIGALRM, &act, 0); 
    sigaction(SIGINT, &act, 0);

    alarm(5); // 设置定时5秒

    printf("程序运行中，可以尝试\n");
    printf("1. 按 Ctrl+C 测试 SIGINT\n"); 
    printf("2. 等待5秒测试 SIGALRM\n");
    
    for (int i = 0; i < 5 ; ++i) {
        pause();  // 是当前进程睡眠，等待任何信号
    }

    return 0;
}