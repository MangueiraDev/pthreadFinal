CC = gcc
CFLAGS = -Wall -pthread -Iinclude
SRC = src/main.c src/task.c src/scheduler.c src/thread_funcs.c
OUT_DIR = build

# Cria pasta de saída se necessário
$(OUT_DIR):
	mkdir -p $(OUT_DIR)

# Round Robin (default)
rr: $(OUT_DIR)
	$(CC) $(CFLAGS) -o $(OUT_DIR)/scheduler_rr $(SRC)

# FIFO (define FIFO_MODE para o compilador)
fifo: $(OUT_DIR)
	$(CC) $(CFLAGS) -DFIFO_MODE -o $(OUT_DIR)/scheduler_fifo $(SRC)

# Limpar
clean:
	rm -rf $(OUT_DIR)