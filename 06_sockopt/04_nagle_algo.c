#include "util_all.h"
#include <netinet/tcp.h> // 必须
#include <unistd.h>

/**
 * Nagle 算法是一种用于提高网络效率的算法，主要解决TCP协议中的"小数据包问题"。
 * 防止因数据包过多而发生网络过载，主要应用于TCP层（默认使用）.
 * 
 * 小数据包场景问题：将每个字母都单独封装成TCP包发送，造成：
 *  - 网络拥塞（每个包都有40字节的IP+TCP头，但数据只有1字节）
 *  - 接收方需要处理大量小包，性能下降
 * 
 * Nagle算法的工作原理：合并小数据包，等待以下条件之一满足时才发送
 *  - 积累到一定数据量（通常是一个MSS，约1460字节）
 *  - 收到前一个包的ACK确认（只有收到前一数据的ACK消息，该算法才能发送下一条数据）
 *  - 超时（通常200ms）
 * 
 * 使用场景：
 *  - 启用：文件传输、批量数据处理
 *  - 禁用：实时游戏、VoIP、交互式应用
*/

int main(int argc, char* argv[]) 
{
    int ret = 0;
    int tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
    
    // 检查当前Nagle状态
    int opt_val;
    socklen_t opt_len = sizeof(opt_val);
    getsockopt(tcp_sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len);
    printf("Nagle算法状态: %s\n", opt_val ? "已禁用" : "已启用");
    
    // 禁用Nagle算法
    int disable = 1;
    ret = setsockopt(tcp_sock, IPPROTO_TCP, TCP_NODELAY, (void*)&disable, sizeof(disable));
    if ( ret == 0) { printf("Nagle算法状态: 等待禁用中...\n"); }
    
    // 验证设置
    getsockopt(tcp_sock, IPPROTO_TCP, TCP_NODELAY, (void*)&opt_val, &opt_len);
    printf("Nagle算法状态: %s\n", opt_val ? "已禁用" : "已启用");
    
    close(tcp_sock);
}