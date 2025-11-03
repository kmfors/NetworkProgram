###############################################################################
# 1. 编译器配置

CC := gcc
CFLAGS := -g -O0 -Wall -I00_UtilTools

###############################################################################
# 2. 目录和源文件配置

# 各目录路径
UTIL_DIR := 00_UtilTools  # 工具类源文件目录
SRC_DIR := 14_epoll       # 主程序源文件目录
OBJ_DIR := obj            # 编译生成的中间文件目录
BIN_DIR := bin            # 可执行文件输出目录

# 主程序源文件名（不含扩展名）
SERVER_BASE := 03_echo_EPET_serv
CLIENT_BASE := 01_echo_client

# 完整源文件路径
SERVER_SRC := $(SRC_DIR)/$(SERVER_BASE).c
CLIENT_SRC := $(SRC_DIR)/$(CLIENT_BASE).c
UTIL_SRCS := $(wildcard $(UTIL_DIR)/*.c)  # 工具目录下所有.c文件

###############################################################################
# 3. 编译目标配置

# 编译后生成的中间文件（.o文件）
SERVER_OBJ := $(OBJ_DIR)/$(SERVER_BASE).o
CLIENT_OBJ := $(OBJ_DIR)/$(CLIENT_BASE).o
UTIL_OBJS := $(patsubst $(UTIL_DIR)/%.c,$(OBJ_DIR)/util_%.o,$(UTIL_SRCS))

# 最终生成的可执行文件
TARGET_SERV := $(BIN_DIR)/serv
TARGET_CLNT := $(BIN_DIR)/clnt

###############################################################################
# 4. 编译规则

# 4.1 默认构建目标：同时生成服务器和客户端程序
all: $(TARGET_SERV) $(TARGET_CLNT)

# 4.2.1 链接生成服务器可执行文件
$(TARGET_SERV): $(SERVER_OBJ) $(UTIL_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 4.2.2 链接生成客户端可执行文件
$(TARGET_CLNT): $(CLIENT_OBJ) $(UTIL_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 4.3 编译主程序源代码为对象文件（%为通配符，$<为源文件，$@为目标文件）
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 4.4 编译工具目录源代码为对象文件（添加util_前缀避免重名冲突）
$(OBJ_DIR)/util_%.o: $(UTIL_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

###############################################################################
# 5. 辅助规则

# 创建输出目录（|表示顺序依赖，只检查存在性，不触发重新构建）
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 清理编译产物
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all clean
