#ifndef _UTIL_ERROR_H_
#define _UTIL_ERROR_H_

// 错误码定义头文件

#define ERR_CODE_BASE 1000
#define PROJECT_CODE_MAP(XX)\
    XX(SUCC_CODE, 0, "成功")\
    XX(ERR_SOCKET_FUNC_CALL, (ERR_CODE_BASE + 1), "socket() error")\
    XX(ERR_BIND_FUNC_CALL, (ERR_CODE_BASE + 2), "bind() error")\
    XX(ERR_LISTEN_FUNC_CALL, (ERR_CODE_BASE + 3), "listen() error")\
    XX(ERR_ACCEPT_FUNC_CALL, (ERR_CODE_BASE + 4), "accept() error")\
    XX(ERR_CONNECT_FUNC_CALL, (ERR_CODE_BASE + 5), "connect() error")\
    XX(ERR_WRITE_FUNC_CALL, (ERR_CODE_BASE + 6), "write() error")\
    XX(ERR_READ_FUNC_CALL, (ERR_CODE_BASE + 7), "read() error")\



typedef enum {
#define XX(name, code, msg) name = code,
    PROJECT_CODE_MAP(XX)
#undef XX
} PROJECT_CODE_ENUM;


#define PROJECT_CODE_MSG(name, code, msg)\
    case name:                           \
        return msg;                      \

const char* ERR_UNKNOW_MSG = "未知错误";
static inline const char* getMsgByCode(int err) {
    switch (err) {
        PROJECT_CODE_MAP(PROJECT_CODE_MSG)
    default:
        return ERR_UNKNOW_MSG;
    }
}

#endif  // ! _UTIL_ERROR_H_