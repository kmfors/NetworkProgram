#include "util_all.h"
#include <unistd.h>

#define TRUE 1
#define FALSE 0
#define BUF_SIZE 30

/**
 * SO_REUSEADDR 是一个非常重要的套接字选项，用于地址重用：
 * 允许在同一端口上快速重启服务器，解决"Address already in use"问题。
 * 
 * 典型场景：
 *  1. 服务器启动，绑定端口 8080
 *  2. 客户端连接...
 *  3. 服务器崩溃或主动关闭
 *  4. 立即重启服务器 → 绑定失败！"Address already in use"
 *  5. 需要等待30秒到2分钟才能重新绑定
 * 
 * 问题原因：TCP连接的TIME_WAIT状态：
 *  - 主动关闭连接的一方会进入TIME_WAIT状态
 *  - 通常持续2MSL（约30秒到2分钟）
 *  - 在此期间，端口被认为仍在使用中
 * 
 * 解决办法：套接字选项设置 SO_REUSEADDR，开发调试时不需要等待TIME_WAIT结束
 * 
 * 回声服务器，可搭配 02_echo_client 程序
*/

// SO_REUSEADDR 默认为假
int main(int argc, char* argv[]) {
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    serv.sock = socket(PF_INET, SOCK_STREAM, 0);

#if 1
    // 解决地址分配错误
    int option = TRUE;
    socklen_t optlen = sizeof(option);
    setsockopt(serv.sock, SOL_SOCKET, SO_REUSEADDR, (void*)&option, optlen);
#endif

    serv.addr.sin_family = AF_INET;
    serv.addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.addr.sin_port = htons(atoi(argv[1]));

    int ret = bind(serv.sock, (struct sockaddr*)&serv.addr, sizeof(serv.addr));
    if (ret == -1) handleError(getMsgByCode(ERR_BIND_FUNC_CALL));

    ret = listen(serv.sock, 5);
    if (ret == -1) handleError(getMsgByCode(ERR_LISTEN_FUNC_CALL));

    clnt.addr_len = sizeof(clnt.addr);
    clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
    if (clnt.sock == -1)  handleError(getMsgByCode(ERR_ACCEPT_FUNC_CALL));

    int str_len = 0;
    char buf[BUF_SIZE] = {0};
    while((str_len = read(clnt.sock, buf, BUF_SIZE)) != 0) {
        write(clnt.sock, buf, str_len);
        write(1, buf, str_len);
    }
    close(clnt.sock);
    close(serv.sock);
    return 0;
}