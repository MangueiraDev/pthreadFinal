// ==========================
// FILE: src/tasks/task_crud_delete.c
// ==========================

#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_delete(void* arg) {
    printf("[DELETE] Iniciando remoção\n");
    remove_usuario(1);
    sleep(1); // simular latência
    printf("[DELETE] Finalizada remoção\n");
    return NULL;
}