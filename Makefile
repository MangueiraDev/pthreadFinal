# ==========================
# FILE: Makefile
# ==========================

CC = gcc
CFLAGS = -Wall -pthread -lsqlite3 -Iinclude
SRC = src
BUILD = build

SRCS = \
	$(SRC)/main.c \
	$(SRC)/task_manager.c \
	$(SRC)/scheduler.c \
	$(SRC)/thread_funcs.c \
	$(SRC)/db/db.c \
	$(SRC)/tasks/task_crud_insert.c \
	$(SRC)/tasks/task_crud_list.c \
	$(SRC)/tasks/task_crud_update.c \
	$(SRC)/tasks/task_crud_delete.c

all:
	make clean
	make fifo && ./build/scheduler_fifo
	make rr && ./build/scheduler_rr

fifo: CFLAGS += -DFIFO_MODE
fifo:
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(SRCS) -o $(BUILD)/scheduler_fifo
	@echo "Executável FIFO gerado: ./$(BUILD)/scheduler_fifo"

rr:
	@mkdir -p $(BUILD)
	$(CC) $(CFLAGS) $(SRCS) -o $(BUILD)/scheduler_rr
	@echo "Executável RR gerado: ./$(BUILD)/scheduler_rr"

clean:
	rm -rf $(BUILD)