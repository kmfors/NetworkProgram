#include "util_all.h"
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
/**
 * 实现IO复用的服务器
 * 
 */

 #define BUF_SIZE 1024

 int main(int argc, char* argv[])
 {
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if (ret != 0)   handleError(getMsgByCode(ret));

    int fd_num = 0, str_len = 0;
    char buf[BUF_SIZE] = {0};
    INIT_STRUCT_FIELD(struct timeval, timeout);
    fd_set reads, cpy_reads;
    FD_ZERO(&reads);
    FD_SET(serv.sock, &reads);
    int fd_max = serv.sock;

    while(1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        fd_num = select(fd_max + 1, &cpy_reads, 0, 0, &timeout);
        if (fd_num == -1) {perror("select error"); break;}
        if (fd_num == 0)  {printf("Timeout...\n"); continue;}

        for (int i = 0; i < fd_max+1; i++) {
            if(FD_ISSET(i, &cpy_reads)) {
                if (i == serv.sock) {
                    clnt.addr_len = sizeof(clnt.addr);
                    clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
                    if (clnt.sock == -1) { perror("accept error"); continue;}
                    FD_SET(clnt.sock, &reads);
                    if(fd_max < clnt.sock)  fd_max = clnt.sock;
                    printf("connected client: %d \n", clnt.sock);
                } else {
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0) { // 关闭请求
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client: %d \n", i);
                    } else if (str_len > 0) {
                        write(i, buf, str_len);  // echo
                    } else { 
                        perror("read error");
                        FD_CLR(i, &reads);
                        close(i);
                    }
                }
            }
        }
    }
    // 关闭所有socket
    for (int i = 0; i <= fd_max; i++) {
        if (FD_ISSET(i, &reads))  close(i);
    }
    return 0;
 }