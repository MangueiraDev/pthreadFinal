#include <stdio.h>
#include <time.h>
#include "task_manager.h"
#include "task.h"

// Funções CRUD
#include "task_crud_insert.h"
#include "task_crud_list.h"
#include "task_crud_update.h"
#include "task_crud_delete.h"

extern Task task_queue[NUM_THREADS];

void init_task_manager() {
    printf("Inicializando tarefas CRUD (8 no total)...\n");

    struct timespec now;
    
    clock_gettime(CLOCK_MONOTONIC, &now);

    // Tarefas de INSERÇÃO
    task_queue[0] = (Task){
        .id = 0,
        .type = APERIODIC,
        .name = "Inserção A (CREATE)",
        .task_func = task_crud_insert,
        .total_work_units = 2,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[0].deadline.tv_sec += 12;

    task_queue[1] = (Task){
        .id = 1,
        .type = PERIODIC,
        .name = "Inserção B (CREATE)",
        .task_func = task_crud_insert,
        .total_work_units = 2,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[1].deadline.tv_sec += 10;

    // Tarefas de LEITURA
    task_queue[2] = (Task){
        .id = 2,
        .type = APERIODIC,
        .name = "Leitura A (READ)",
        .task_func = task_crud_list,
        .total_work_units = 1,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[2].deadline.tv_sec += 11;

    task_queue[3] = (Task){
        .id = 3,
        .type = PERIODIC,
        .name = "Leitura B (READ)",
        .task_func = task_crud_list,
        .total_work_units = 1,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[3].deadline.tv_sec += 15;

    // Tarefas de ATUALIZAÇÃO
    task_queue[4] = (Task){
        .id = 4,
        .type = APERIODIC,
        .name = "Atualização A (UPDATE)",
        .task_func = task_crud_update,
        .total_work_units = 2,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[4].deadline.tv_sec += 6;

    task_queue[5] = (Task){
        .id = 5,
        .type = PERIODIC,
        .name = "Atualização B (UPDATE)",
        .task_func = task_crud_update,
        .total_work_units = 2,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[5].deadline.tv_sec += 4;

    // Tarefas de REMOÇÃO
    task_queue[6] = (Task){
        .id = 6,
        .type = APERIODIC,
        .name = "Remoção A (DELETE)",
        .task_func = task_crud_delete,
        .total_work_units = 1,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[6].deadline.tv_sec += 12;

    task_queue[7] = (Task){
        .id = 7,
        .type = PERIODIC,
        .name = "Remoção B (DELETE)",
        .task_func = task_crud_delete,
        .total_work_units = 1,
        .progress = 0,
        .missed_deadline = false,
        .deadline = now
    };
    task_queue[7].deadline.tv_sec += 14;

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("  - Tarefa %d: %s | Tipo: %s | Deadline: +%lds\n",
               i,
               task_queue[i].name,
               task_queue[i].type == PERIODIC ? "Periódica" : "Não-periódica",
               task_queue[i].deadline.tv_sec - now.tv_sec);
    }
}