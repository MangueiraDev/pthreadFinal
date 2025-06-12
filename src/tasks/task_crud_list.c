// ==========================
// FILE: src/tasks/task_crud_list.c
// ==========================

#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_list(void* arg) {
    printf("[LIST] Listando usuários\n");
    listar_usuarios();
    sleep(1);
    return NULL;
}