#include "util_all.h"
#include <unistd.h>
#include <pthread.h>

/**
 *
 * 销毁线程的3中方法：
 *  - pthread_join 函数：调用该函数的线程将进入阻塞状态
 *  - pthread_detach 函数：线程分离，调用后不陷入阻塞
*/

#define BUF_SIZE 100
#define MAX_CLNT 256

pthread_mutex_t g_mutx;
int g_clnt_cnt = 0;
pthread_t g_thd_id = 0;
int g_clnt_socks[MAX_CLNT] = {0};

void send_msg(char* msg, int len) {
    pthread_mutex_lock(&g_mutx);
    for(int i = 0; i < g_clnt_cnt; i++) {
        write(g_clnt_socks[i], msg, len);
    }
    pthread_mutex_unlock(&g_mutx);
}

void* handle_clnt(void* arg) {
    int clnt_sock = *((int*)arg);
    int str_len = 0;
    char msg[BUF_SIZE] = {0};

    while((str_len = read(clnt_sock, msg, BUF_SIZE)) != 0)
        send_msg(msg, str_len);

    pthread_mutex_lock(&g_mutx);
    for(int i = 0; i < g_clnt_cnt; i++) {
        if (clnt_sock == g_clnt_socks[i]) {
            while(i++ < g_clnt_cnt - 1) {
                g_clnt_socks[i] = g_clnt_socks[i + 1];
            }
            break;
        }
    }
    g_clnt_cnt--;
    pthread_mutex_unlock(&g_mutx);
    close(clnt_sock);
    return NULL;
}

int main(int argc, char* argv[])
{
    ASSERT_ARGC_SERVER(argc);

    pthread_mutex_init(&g_mutx, NULL);
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if(ret != 0)  handleError(getMsgByCode(ret));

    pthread_t t_id;
    while(1) {
        clnt.addr_len = sizeof(clnt.addr);
        clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);

        pthread_mutex_lock(&g_mutx);
        g_clnt_socks[g_clnt_cnt++] = clnt.sock;
        pthread_mutex_unlock(&g_mutx);

        pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt.sock);
        pthread_detach(t_id);
        printf("Connected client IP: %s \n", inet_ntoa(clnt.addr.sin_addr));
    }
    close(serv.sock);
    return 0;



}