#include "util_all.h"
#include <pthread.h>

#define BUF_SIZE 1024
#define SMALL_BUF 100

void* request_handler(void* arg);
void send_data(FILE* fp, char* ct, char* file_name);
char* content_type(char* file);
void send_error(FILE* fp);


int main(int argc, char* argv[])
{
    ASSERT_ARGC_SERVER(argc);

    INIT_STRUCT_FIELD(serv_sock_info_t, serv);
    INIT_STRUCT_FIELD(clnt_sock_info_t, clnt);

    int ret = tcp_listen_func(argv[1], &serv, &clnt);
    if(ret != 0)    handleError(getMsgByCode(ret));

    //char buf[BUF_SIZE] = {0};
    pthread_t t_id;
    while(1) {
        clnt.addr_len = sizeof(clnt.addr);
        clnt.sock = accept(serv.sock, (struct sockaddr*)&clnt.addr, &clnt.addr_len);
        printf("Connection Request: %s:%d\n", inet_ntoa(clnt.addr.sin_addr),
            ntohs(clnt.addr.sin_port));

        pthread_create(&t_id, NULL, request_handler, &clnt.sock);
        pthread_detach(t_id);
    }
    close(serv.sock);
    return 0;
}

void* request_handler(void* arg)
{
    int sock = *((int*)arg);
    char req_line[SMALL_BUF] = {0};

    FILE* fread = fdopen(sock, "r");
    FILE* fwrite = fdopen(dup(sock), "w");
    fgets(req_line, SMALL_BUF, fread);
    if(strstr(req_line, "HTTP/") == NULL) {
        send_error(fwrite);
        fclose(fread);
        fclose(fwrite);
        return ;
    }
    char ct[15] = {0};
    char method[10] = {0};
    char file_name[30] = {0};
    strcpy(method, strtok(req_line, " /"));
    strcpy(file_name, strtok(NULL, " /"));
    strcpy(ct, content_type(file_name));
    if(strcmp(method, "GET") != 0) {
        send_error(fwrite);
        fclose(fread);
        fclose(fwrite);
        return;
    }
    fclose(fread);
    send_data(fwrite, ct, file_name);
}

void send_data(FILE* fp, char* ct, char* file_name) {
    char protocol[] = "HTTP/1.0 200 ok\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[SMALL_BUF];
    char buf[BUF_SIZE];
    FILE* send_file;

    sprintf(cnt_type, "Content-type:%s\r\n\r\n", ct);
    send_file = fopen(file_name, "r");
    if(send_file == NULL) {
        send_error(fp);
        return;
    }
    // 传输头信息
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);

    while(fgets(buf, BUF_SIZE, send_file) != NULL) {
        fputs(buf, fp);
        fflush(fp);
    }
}

char* content_type(char* file) {
    char extension[SMALL_BUF];
    char file_name[SMALL_BUF];
    strcpy(file_name, file);
    strtok(file_name, ".");
    strcpy(extension, strtok(NULL, "."));

    if(!strcmp(extension, "html") || strcmp(extension, "htm"))
        return "text/html";
    else
        return "text/plain";
}

void send_error(FILE* fp) {
    char protocol[] = "HTTP/1.0 400 Bad Request\r\n";
    char server[] = "Server:Linux Web Server \r\n";
    char cnt_len[] = "Content-length:2048\r\n";
    char cnt_type[] = "Content-type:text/html\r\n\r\n";
    char content[] = "<html><head><title>NETWORK</title></head>"
        "<body><font size=+5><br>发生错误！查看请求文件名和请求方式！"
        "</font></body></html>";
    fputs(protocol, fp);
    fputs(server, fp);
    fputs(cnt_len, fp);
    fputs(cnt_type, fp);
    fflush(fp);
}