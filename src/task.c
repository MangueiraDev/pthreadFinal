// ==========================
// FILE: src/task.c
// ==========================
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "task.h"

void do_work_rr(Task* task, int units_to_do) {
    printf("Thread %d: Retomando trabalho. Progresso atual: %d/%d.\n",
           task->id, task->progress, task->total_work_units);

    int units_done_this_turn = 0;
    for (int i = 0; i < units_to_do && task->progress < task->total_work_units; ++i) {
        sleep(1);
        task->progress++;
        units_done_this_turn++;
        printf("  -> Thread %d: Progresso: %d/%d\n", task->id, task->progress, task->total_work_units);
    }
    printf("Thread %d: Trabalhou por %d unidades neste quantum.\n", task->id, units_done_this_turn);
}

void do_work_fifo(int thread_id) {
    printf("Thread %d: Iniciando trabalho...\n", thread_id);
    int work_time = thread_id + (rand() % 6);
    sleep(work_time);
    printf("Thread %d: Trabalho concluído. Tempo de processamento: %d segundos.\n", thread_id, work_time);
}
