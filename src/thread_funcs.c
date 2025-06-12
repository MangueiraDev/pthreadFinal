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

        if ((finish.tv_sec > deadline.tv_sec) ||
            (finish.tv_sec == deadline.tv_sec && finish.tv_nsec > deadline.tv_nsec)) {
            task_queue[thread_id].missed_deadline = true;
            printf("⚠️  Tarefa %d VIOLOU o deadline!\n", thread_id);
        } else {
            printf("✅ Tarefa %d dentro do deadline.\n", thread_id);
        }

        pthread_cond_signal(&cond_scheduler_wait);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}