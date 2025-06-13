// ==========================
// FILE: include/task.h
// ==========================
#ifndef TASK_H
#define TASK_H

#include <pthread.h>
#include <time.h>
#include <stdbool.h>

typedef enum {
    PERIODIC,
    APERIODIC
} TaskType;

typedef struct {
    int id;
    TaskType type;
    int total_work_units;
    int progress;
    pthread_t thread;
    void* (*task_func)(void*);
    struct timespec deadline;
    struct timespec time_zero;
    struct timespec start_time;
    struct timespec finish_time;
    bool missed_deadline;
    const char* name;  // Nome descritivo da tarefa
} Task;

#define NUM_THREADS 8  // 4 CRUD x (1 periódica + 1 aperiódica)

#endif