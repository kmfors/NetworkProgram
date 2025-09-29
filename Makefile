# 编译器
CC := gcc
CFLAGS := -g -O0 -Wall

# 目录设置
SRC_DIR := 02_DataHandle
OBJ_DIR := obj
BIN_DIR := bin

# 文件名定义（只需要修改这里）
#SERVER_BASE := 01_create_tcp_server
SERVER_BASE := 02_echo_server
CLIENT_BASE := 02_echo_client_pro

# 源文件路径
SERVER_SRC := $(SRC_DIR)/$(SERVER_BASE).c
CLIENT_SRC := $(SRC_DIR)/$(CLIENT_BASE).c

# 目标文件路径
SERVER_OBJ := $(OBJ_DIR)/$(SERVER_BASE).o
CLIENT_OBJ := $(OBJ_DIR)/$(CLIENT_BASE).o

# 可执行程序
TARGET_SERV := $(BIN_DIR)/serv
TARGET_CLNT := $(BIN_DIR)/clnt

# 默认目标
all: $(TARGET_SERV) $(TARGET_CLNT)

# 生成服务器程序
$(TARGET_SERV): $(SERVER_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 生成客户端程序
$(TARGET_CLNT): $(CLIENT_OBJ) | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 编译目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 创建目录
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)/*

.PHONY: all clean
