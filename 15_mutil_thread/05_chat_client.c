#include "util_all.h"
#include <unistd.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 256

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE] = {0};

void* send_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE] = {0};
    while(1) {
        fgets(msg, BUF_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(sock); exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(sock, name_msg, strlen(name_msg));
    }
    return NULL;
}
void* recv_msg(void* arg) {
    int sock = *((int*)arg);
    char name_msg[NAME_SIZE + BUF_SIZE] = {0};
    int str_len = 0;
    while(1) {
        str_len = read(sock, name_msg, NAME_SIZE+BUF_SIZE-1);
        if(str_len == -1)   return (void*)-1;
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
    return NULL;
}



int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Usage: %s <IP> <port> <name>\n", argv[0]);
        return 1;
    }
    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    int ret = tcp_client_handle(argv[1], argv[2], &serv);
    if(ret != 0) handleError(getMsgByCode(ret));

    void* thread_return;
    sprintf(name, "[%s]", argv[3]);
    pthread_t snd_thread, rcv_thread;
    pthread_create(&snd_thread, NULL, send_msg, (void*)&serv.sock);
    pthread_create(&rcv_thread, NULL, recv_msg, (void*)&serv.sock);
    pthread_join(snd_thread, &thread_return);
    pthread_join(rcv_thread, &thread_return);
    close(serv.sock);
    return 0;
}