// ==========================
// FILE: src/tasks/task_crud_update.c
// ==========================

#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_update(void* arg) {
    printf("[UPDATE] Iniciando atualização\n");
    atualiza_usuario(1, "Maria Silva", "maria.silva@example.com");
    sleep(1); // simula tempo de processamento
    printf("[UPDATE] Finalizada atualização\n");
    return NULL;
}