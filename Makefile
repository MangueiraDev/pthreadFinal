# ==========================
# FILE: Makefile
# ==========================

CC = gcc
CFLAGS = -Wall -pthread -lsqlite3 -Iinclude
SRC_DIR = src
BUILD_DIR = build

# Fontes
SRCS_COMMON = \
	$(SRC_DIR)/main.c \
	$(SRC_DIR)/task_manager.c \
	$(SRC_DIR)/scheduler.c \
	$(SRC_DIR)/thread_funcs.c \
	$(SRC_DIR)/db/db.c \
	$(SRC_DIR)/tasks/task_crud_insert.c \
	$(SRC_DIR)/tasks/task_crud_update.c \
	$(SRC_DIR)/tasks/task_crud_delete.c \
	$(SRC_DIR)/tasks/task_crud_list.c

# FIFO build
fifo: CFLAGS += -DFIFO_MODE
fifo: $(SRCS_COMMON)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/scheduler_fifo
	@echo "Executável FIFO gerado: ./$(BUILD_DIR)/scheduler_fifo"

# Round Robin build
rr: $(SRCS_COMMON)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $(BUILD_DIR)/scheduler_rr
	@echo "Executável RR gerado: ./$(BUILD_DIR)/scheduler_rr"

# Limpeza
clean:
	rm -rf $(BUILD_DIR)