// ==========================
// FILE: src/tasks/task_crud_list.c
// ==========================
#include <stdlib.h>

#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_list(void* arg) {
    printf("[LIST] Listando usuários\n");
    listar_usuarios();
    // Simular latência aleatória entre 2 e 12 segundos
    int latency = (rand() % 6) + 2; // 2 a 12 segundos
    sleep(latency);
    return NULL;
}