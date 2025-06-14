// ==========================
// FILE: src/tasks/task_crud_delete.c
// ==========================
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_delete(void* arg) {
    printf("[DELETE] Iniciando remoção\n");
    remove_usuario(1);
    // Simular latência aleatória entre 2 e 12 segundos
    int latency = (rand() % 6) + 2; // 2 a 12 segundos
    sleep(latency);
    printf("[DELETE] Finalizada remoção\n");
    return NULL;
}
