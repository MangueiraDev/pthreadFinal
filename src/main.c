// ==========================
// FILE: src/main.c
// ==========================
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "task.h"
#include "scheduler.h"
#include "thread_funcs.h"

Task task_queue[NUM_THREADS];
int tasks_completed_count = 0;
bool task_done[NUM_THREADS];
int current_turn_id = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_scheduler_wait;
pthread_cond_t cond_thread_work[NUM_THREADS];

int main() {
    srand(time(NULL));

#ifdef FIFO_MODE
    printf("--- Modo FIFO ---\n");
#else
    printf("--- Modo Round Robin ---\n");
#endif

    for (int i = 0; i < NUM_THREADS; i++) {
        task_queue[i].id = i;
        task_queue[i].total_work_units = 3 + rand() % 6;
        task_queue[i].progress = 0;
        printf("  - Tarefa %d criada com %d unidades de trabalho.\n", i, task_queue[i].total_work_units);
    }

    pthread_cond_init(&cond_scheduler_wait, NULL);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_init(&cond_thread_work[i], NULL);
    }

#ifdef FIFO_MODE
    run_fifo_scheduler();
#else
    for (int i = 0; i < NUM_THREADS; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&task_queue[i].thread, NULL, round_robin_thread_func, thread_id);
    }
    run_round_robin_scheduler();
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(task_queue[i].thread, NULL);
    }
#endif

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_scheduler_wait);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_destroy(&cond_thread_work[i]);
    }

    return 0;
}
