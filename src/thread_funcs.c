// ==========================
// FILE: src/thread_funcs.c
// ==========================
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "task.h"

void* round_robin_thread_func(void* arg) {
    int my_id = *(int*)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    while (1) {
        pthread_cond_wait(&cond_thread_work[my_id], &mutex);

        if (task_queue[my_id].progress >= task_queue[my_id].total_work_units)
            break;

        printf("\n=> Thread %d: Assumindo CPU. Quantum: %d unidades.\n", my_id, TIME_QUANTUM);
        do_work_rr(&task_queue[my_id], TIME_QUANTUM);

        if (task_queue[my_id].progress >= task_queue[my_id].total_work_units) {
            printf("**** Thread %d: TAREFA CONCLUÍDA! ****\n", my_id);
            tasks_completed_count++;
        } else {
            printf("Thread %d: Quantum finalizado, tarefa pausada.\n", my_id);
        }

        pthread_cond_signal(&cond_scheduler_wait);
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}

void* fifo_thread_func(void* arg) {
    int thread_id = *(int*)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    while (thread_id != current_turn_id) {
        pthread_cond_wait(&cond_thread_work[thread_id], &mutex);
    }

    printf("Thread %d: Entrou na seção crítica.\n", thread_id);
    do_work_fifo(thread_id);
    printf("Thread %d: Saindo da seção crítica.\n\n", thread_id);

    current_turn_id++;
    if (current_turn_id < NUM_THREADS) {
        pthread_cond_signal(&cond_thread_work[current_turn_id]);
    }

    pthread_mutex_unlock(&mutex);
    return NULL;
}
