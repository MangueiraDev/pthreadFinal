#include <stdio.h>
#include <time.h>
#include "task_manager.h"
#include "task.h"
#include "task_crud_insert.h"
#include "task_crud_list.h"
#include "task_crud_update.h"
#include "task_crud_delete.h"

extern Task task_queue[NUM_THREADS];

void init_task_manager() {
    printf("Inicializando tarefas CRUD...\n");

    struct timespec now;
    clock_gettime(CLOCK_MONOTONIC, &now);

#ifdef FIFO_MODE
    task_queue[0].id = 0;
    task_queue[0].type = APERIODIC;
    task_queue[0].task_func = task_crud_insert;
    task_queue[0].total_work_units = 2;
    task_queue[0].progress = 0;
    task_queue[0].missed_deadline = false;
    task_queue[0].deadline = now;
    task_queue[0].deadline.tv_sec += 2;

    task_queue[1].id = 1;
    task_queue[1].type = PERIODIC;
    task_queue[1].task_func = task_crud_list;
    task_queue[1].total_work_units = 1;
    task_queue[1].progress = 0;
    task_queue[1].missed_deadline = false;
    task_queue[1].deadline = now;
    task_queue[1].deadline.tv_sec += 1;
#else
    task_queue[0].id = 0;
    task_queue[0].type = PERIODIC;
    task_queue[0].task_func = task_crud_update;
    task_queue[0].total_work_units = 2;
    task_queue[0].progress = 0;
    task_queue[0].missed_deadline = false;
    task_queue[0].deadline = now;
    task_queue[0].deadline.tv_sec += 1;

    task_queue[1].id = 1;
    task_queue[1].type = APERIODIC;
    task_queue[1].task_func = task_crud_delete;
    task_queue[1].total_work_units = 2;
    task_queue[1].progress = 0;
    task_queue[1].missed_deadline = false;
    task_queue[1].deadline = now;
    task_queue[1].deadline.tv_sec += 3;
#endif

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("  - Tarefa %d atribuída (%s), deadline em +%ld s\n", i,
               task_queue[i].type == PERIODIC ? "periódica" : "não-periódica",
               task_queue[i].deadline.tv_sec - now.tv_sec);
    }
}