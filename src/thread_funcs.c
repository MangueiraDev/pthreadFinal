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


void* do_work_rr_b(Task *task, int units_to_do)
{
    printf("Thread %d: Retomando trabalho. Progresso atual: %d/%d.\n",
           task->id, task->progress, task->total_work_units);

    int units_done_this_turn = 0;
    for (int i = 0; i < units_to_do; ++i)
    {
        if (task->progress >= task->total_work_units)
            break;

        sleep(1); // Simula o tempo de trabalho

        task->progress++;

        units_done_this_turn++;

        printf("  -> Thread %d: Progresso: %d/%d\n", task->id, task->progress, task->total_work_units);
    }
    printf("Thread %d: Trabalhou por %d unidades neste quantum.\n", task->id, units_done_this_turn);

    return NULL;
}



void* round_robin_thread_func(void* arg) {
    int thread_id = *(int*)arg;
    free(arg);
    pthread_mutex_lock(&mutex);
    while (1) {

        pthread_cond_wait(&cond_thread_work[thread_id], &mutex);

        // Se a tarefa já está completa, apenas sai.
        if (task_queue[thread_id].progress >= task_queue[thread_id].total_work_units) {
            break;
        }

        printf("🧵 Executando tarefa %d: %s\n", thread_id, task_queue[thread_id].name);
        
        clock_gettime(CLOCK_MONOTONIC, &task_queue[thread_id].start_time);

        do_work_rr_b(&task_queue[thread_id], TIME_QUANTUM);
       // if (task_queue[thread_id].task_func != NULL) {
        //    task_queue[thread_id].task_func(NULL);
       // }

        
         // Verifica se a tarefa foi concluída NESTE TURNO
        if (task_queue[thread_id].progress >= task_queue[thread_id].total_work_units) {

            clock_gettime(CLOCK_MONOTONIC, &task_queue[thread_id].finish_time);


            printf("**** Thread %s: TAREFA CONCLUÍDA! ****\n", task_queue[thread_id].name);
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

        } else {
            printf("Thread %s: Quantum finalizado, tarefa pausada.\n", task_queue[thread_id].name);
        }

        pthread_cond_signal(&cond_scheduler_wait);
    }

    pthread_mutex_lock(&mutex);


    return NULL;
}


