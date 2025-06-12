// ==========================
// FILE: src/scheduler.c
// ==========================
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "task.h"
#include "thread_funcs.h"

#ifdef FIFO_MODE

void run_fifo_scheduler() {
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_init(&cond_thread_work[i], NULL);
        task_done[i] = false;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        int* thread_id = malloc(sizeof(int));
        *thread_id = i;
        pthread_create(&task_queue[i].thread, NULL, fifo_thread_func, thread_id);
    }

    sleep(1);
    pthread_cond_signal(&cond_thread_work[0]);

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(task_queue[i].thread, NULL);
    }

    printf("\n--- FIFO: Todas as tarefas foram concluídas! Encerrando. ---\n");
}

#else

void run_round_robin_scheduler() {
    sleep(1);
    pthread_mutex_lock(&mutex);

    while (tasks_completed_count < NUM_THREADS) {
        Task* current_task = &task_queue[current_turn_id];

        if (current_task->progress < current_task->total_work_units) {
            pthread_cond_signal(&cond_thread_work[current_turn_id]);
            pthread_cond_wait(&cond_scheduler_wait, &mutex);
        }

        current_turn_id = (current_turn_id + 1) % NUM_THREADS;
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_signal(&cond_thread_work[i]);
    }

    pthread_mutex_unlock(&mutex);
    printf("\n--- RR: Todas as tarefas foram concluídas! Encerrando. ---\n");
}

#endif
