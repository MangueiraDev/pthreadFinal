#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include "task.h"
#include "thread_funcs.h"

extern Task task_queue[NUM_THREADS];
extern pthread_mutex_t mutex;
extern pthread_cond_t cond_thread_work[NUM_THREADS];
extern pthread_cond_t cond_scheduler_wait;
extern bool task_done[NUM_THREADS];
extern int current_turn_id;
extern int tasks_completed_count;

#ifdef FIFO_MODE

void run_fifo_scheduler()
{
    printf("\n[FIFO] Iniciando escalonamento FIFO com tarefas reais...\n");

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_cond_init(&cond_thread_work[i], NULL);
        task_done[i] = false;
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        printf("🧵 Executando tarefa %d: %s\n", i, task_queue[i].name);
        clock_gettime(CLOCK_MONOTONIC, &task_queue[i].start_time);
        pthread_create(&task_queue[i].thread, NULL, task_queue[i].task_func, NULL);
        pthread_join(task_queue[i].thread, NULL);
        clock_gettime(CLOCK_MONOTONIC, &task_queue[i].finish_time);

        if ((task_queue[i].finish_time.tv_sec > task_queue[i].deadline.tv_sec) ||
            (task_queue[i].finish_time.tv_sec == task_queue[i].deadline.tv_sec &&
             task_queue[i].finish_time.tv_nsec > task_queue[i].deadline.tv_nsec))
        {
            task_queue[i].missed_deadline = true;
            printf("⚠️  %s VIOLOU o deadline!\n", task_queue[i].name);
        }
        else
        {
            printf("✅ %s dentro do deadline.\n", task_queue[i].name);
        }
    }

    printf("\n[FIFO] Todas as tarefas foram concluídas!\n");
    for (int i = 0; i < NUM_THREADS; i++)
    {
        double start = task_queue[i].start_time.tv_sec + task_queue[i].start_time.tv_nsec / 1e9;
        double finish = task_queue[i].finish_time.tv_sec + task_queue[i].finish_time.tv_nsec / 1e9;
        double exec = finish - start;
        printf("⏱️  %s | Execução: %.6fs | Deadline: %ld.%09ld | %s\n",
               task_queue[i].name,
               exec,
               task_queue[i].deadline.tv_sec,
               task_queue[i].deadline.tv_nsec,
               task_queue[i].missed_deadline ? "❌ Violado" : "✅ OK");
    }
}

#else

void run_round_robin_scheduler()
{
    printf("\n[RR] Iniciando escalonamento Round Robin com tarefas reais...\n");

    sleep(1);
    pthread_mutex_lock(&mutex);
    while (tasks_completed_count < NUM_THREADS)
    {
        if (!task_done[current_turn_id])
        {
            printf("[RR] Turno da tarefa %d: %s\n", current_turn_id, task_queue[current_turn_id].name);
            pthread_cond_signal(&cond_thread_work[current_turn_id]);
            pthread_cond_wait(&cond_scheduler_wait, &mutex);
        }

        current_turn_id = (current_turn_id + 1) % NUM_THREADS;
    }

    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_cond_signal(&cond_thread_work[i]);
    }
    pthread_mutex_unlock(&mutex);

    printf("\n[RR] Todas as tarefas foram concluídas!\n");
    for (int i = 0; i < NUM_THREADS; i++)
    {
        double start = task_queue[i].start_time.tv_sec + task_queue[i].start_time.tv_nsec / 1e9;
        double finish = task_queue[i].finish_time.tv_sec + task_queue[i].finish_time.tv_nsec / 1e9;
        double exec = finish - start;
        printf("⏱️  %s | Execução: %.6fs | ld.%09ld | %s\n",
               task_queue[i].name,
               exec,
               task_queue[i].deadline.tv_sec,
               task_queue[i].deadline.tv_nsec,
               task_queue[i].missed_deadline ? "❌ Violado" : "✅ OK");
    }
}

#endif