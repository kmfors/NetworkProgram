# 编译器
CC := gcc
CFLAGS := -g -O0 -Wall -I00_UtilTools

# 目录设置
UTIL_DIR := 00_UtilTools
SRC_DIR := 14_epoll
OBJ_DIR := obj
BIN_DIR := bin

# 文件名定义（只需要修改这里）
SERVER_BASE := 03_echo_EPET_serv
CLIENT_BASE := 01_echo_client

# 源文件路径
SERVER_SRC := $(SRC_DIR)/$(SERVER_BASE).c
CLIENT_SRC := $(SRC_DIR)/$(CLIENT_BASE).c

# 查找00_UtilTools目录中的所有.c文件（不包括main函数文件）
UTIL_SRCS := $(wildcard $(UTIL_DIR)/*.c)

##########################################################################
# 目标文件路径
SERVER_OBJ := $(OBJ_DIR)/$(SERVER_BASE).o
CLIENT_OBJ := $(OBJ_DIR)/$(CLIENT_BASE).o
UTIL_OBJS := $(patsubst $(UTIL_DIR)/%.c,$(OBJ_DIR)/util_%.o,$(UTIL_SRCS))

# 可执行程序
TARGET_SERV := $(BIN_DIR)/serv
TARGET_CLNT := $(BIN_DIR)/clnt

##########################################################################
# 默认目标
all: $(TARGET_SERV) $(TARGET_CLNT)

# 生成服务器程序
$(TARGET_SERV): $(SERVER_OBJ) $(UTIL_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 生成客户端程序
$(TARGET_CLNT): $(CLIENT_OBJ) $(UTIL_OBJS) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 编译主程序目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 编译工具目录目标文件 - 使用不同的文件名模式
$(OBJ_DIR)/util_%.o: $(UTIL_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 创建目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(OBJ_DIR)/* $(BIN_DIR)/*

.PHONY: all clean
