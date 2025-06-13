#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include "task.h"

extern Task task_queue[NUM_THREADS];
extern pthread_mutex_t mutex;
extern pthread_cond_t cond_scheduler_wait;
extern pthread_cond_t cond_thread_work[NUM_THREADS];
extern int current_turn_id;
extern bool task_done[NUM_THREADS];
extern int tasks_completed_count;

void* round_robin_thread_func(void* arg) {
    int thread_id = *(int*)arg;
    free(arg);

    while (1) {
        pthread_mutex_lock(&mutex);
        while (current_turn_id != thread_id && !task_done[thread_id]) {
            pthread_cond_wait(&cond_thread_work[thread_id], &mutex);
        }

        if (task_done[thread_id]) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        pthread_mutex_unlock(&mutex);

        printf("🧵 Executando tarefa %d: %s\n", thread_id, task_queue[thread_id].name);
        
        clock_gettime(CLOCK_MONOTONIC, &task_queue[thread_id].start_time);

        if (task_queue[thread_id].task_func != NULL) {
            task_queue[thread_id].task_func(NULL);
        }

        clock_gettime(CLOCK_MONOTONIC, &task_queue[thread_id].finish_time);

        pthread_mutex_lock(&mutex);
        task_done[thread_id] = true;
        tasks_completed_count++;

        struct timespec finish = task_queue[thread_id].finish_time;
        struct timespec deadline = task_queue[thread_id].deadline;
        double time_exec = finish.tv_sec - task_queue[thread_id].start_time.tv_sec;

        if ((time_exec) > (deadline.tv_sec - task_queue[thread_id].time_zero.tv_sec)) {
            task_queue[thread_id].missed_deadline = true;
            printf("⚠️  %s VIOLOU o deadline! | Tempo de execução: %f\n", task_queue[thread_id].name, time_exec);
        } else {
            printf("✅ %s dentro do deadline. | Tempo de execução: %f\n", task_queue[thread_id].name, time_exec);
        }

        pthread_cond_signal(&cond_scheduler_wait);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}