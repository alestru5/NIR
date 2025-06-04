CC = gcc
CFLAGS = -Wall -Wextra -I./include -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -ljson-c -lpthread -lsqlite3
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Создаем директории при необходимости
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Находим все .c файлы в src директории
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/detector

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) 