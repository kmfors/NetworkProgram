
#ifndef _UTIL_TOOL_H_
#define _UTIL_TOOL_H_

// 实用工具头文件

#include <stdio.h> 
#include <string.h>
#include <stdlib.h>


/**
 * #define	EXIT_FAILURE	1	Failing exit status.  
 * #define	EXIT_SUCCESS	0	Successful exit status. 
*/

#ifndef ASSERT_ARGC_SERVER
#define ASSERT_ARGC_SERVER(argc)    \
    if (argc != 2) {                \
        printf("Usage: %s <port>\n", argv[0]);    \
        exit(EXIT_FAILURE);         \
    }
#endif

#ifndef ASSERT_ARGC_CLIENT
#define ASSERT_ARGC_CLIENT(argc)    \
    if (argc != 3) {                \
        printf("Usage: %s <IP> <port>\n", argv[0]);   \
        exit(EXIT_FAILURE);         \
    }
#endif
    
#ifndef INIT_STRUCT_FIELD
#define INIT_STRUCT_FIELD(Type, field) \
    Type field;                 \
    memset(&field, 0, sizeof(field));
#endif

// 错误消息处理
static inline void handleError(const char* msg) {
    fputs(msg, stderr);     // 将字符串打印至错误输出
    fputc('\n', stderr);    // 将单个字符打印至错误输出
    exit(EXIT_FAILURE);     // Failing exit
}



#endif  // ! _UTIL_TOOL_H_