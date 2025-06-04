CC = gcc
CFLAGS = -Wall -Wextra -I../include -I/opt/homebrew/include
LDFLAGS = -L/opt/homebrew/lib -ljson-c -lpthread -lsqlite3

# Исходные файлы проекта
SRC_DIR = ../src
PROJECT_SRCS = $(filter-out $(SRC_DIR)/main.c,$(wildcard $(SRC_DIR)/*.c))
PROJECT_OBJS = $(notdir $(PROJECT_SRCS:.c=.o))

# Тестовые файлы
TEST_SRCS = main_test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

# Целевые файлы
TEST_BIN = run_tests

.PHONY: all clean test

all: $(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS) $(PROJECT_OBJS)
	$(CC) $(TEST_OBJS) $(PROJECT_OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_BIN)
	./$(TEST_BIN)

clean:
	rm -f $(TEST_OBJS) $(PROJECT_OBJS) $(TEST_BIN) 