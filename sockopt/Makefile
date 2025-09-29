# 1、编译器
CC := gcc
# 2、编译选项
CFLAGS := -g -O0 -Wall

# 源文件路径
SRC_DIR := 01_BaseNetwork
# 目标文件路径
OBJ_DIR := obj
# 可执行程序路径
BIN_DIR := bin

# 源文件列表
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# 目标文件列表
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# 可执行程序的名称（根据实际需要调整）
TARGET_SERV := $(BIN_DIR)/serv
TARGET_CLNT := $(BIN_DIR)/clnt

# 默认目标 - 编译所有程序
all: $(TARGET_SERV) $(TARGET_CLNT)

# 生成服务器程序
$(TARGET_SERV): $(OBJ_DIR)/serv.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 生成客户端程序  
$(TARGET_CLNT): $(OBJ_DIR)/clnt.o | $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# 或者如果你只有一个程序：
# $(TARGET_SERV): $(OBJ_FILES) | $(BIN_DIR)
# 	$(CC) $(CFLAGS) $^ -o $@

# 编译生成目标文件
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# 创建目标文件夹
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# 清理规则
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# 安装规则（可选）
install: all
	cp $(TARGET_SERV) $(TARGET_CLNT) /usr/local/bin/

# 声明伪目标
.PHONY: all clean install