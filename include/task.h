// ==========================
// FILE: include/task.h
// ==========================
#ifndef TASK_H
#define TASK_H

#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define NUM_THREADS 5
#define TIME_QUANTUM 2

typedef struct {
    pthread_t thread;
    int id;
    int total_work_units;
    int progress;
} Task;

extern Task task_queue[NUM_THREADS];
extern int tasks_completed_count;
extern int current_turn_id;
extern bool task_done[NUM_THREADS];

extern pthread_mutex_t mutex;
extern pthread_cond_t cond_scheduler_wait;
extern pthread_cond_t cond_thread_work[NUM_THREADS];

void do_work_rr(Task* task, int units_to_do);
void do_work_fifo(int thread_id);

#endif
