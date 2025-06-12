// ==========================
// FILE: src/tasks/task_crud_insert.c
// ==========================

#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_insert(void* arg) {
    printf("[INSERT] Iniciando tarefa de inserção\n");
    insere_usuario("Maria", "maria@example.com");
    sleep(1); // simular tempo de execução
    printf("[INSERT] Finalizada inserção\n");
    return NULL;
}