#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void* thread_main(void *arg) {
    int cnt = *((int*)arg);
    char* msg = (char*)malloc(sizeof(char)*50);
    strcpy(msg, "Hello, I'm thread~ \n");

    for(int i = 0; i < cnt; i++) {
        sleep(1); puts("running thread");
    }
    return (void*)msg;
}

int main(int argc, char* argv[]) {
    pthread_t t_id;
    int thread_param = 5;
    void* thread_ret;

    int ret = pthread_create(&t_id, NULL, thread_main, (void*)&thread_param);
    if (ret != 0) { puts("pthread_create error"); return -1; }

    ret = pthread_join(t_id, &thread_ret);
    if (ret != 0) { puts("pthread_join error"); return -1; }

    printf("Thread return message: %s \n", (char*)thread_ret);
    free(thread_ret);
    return 0;
}