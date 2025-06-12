#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/task.h"

extern Task task_queue[NUM_THREADS];

void salvar_relatorio_csv(const char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        perror("Erro ao criar arquivo CSV");
        return;
    }

    fprintf(fp, "ID,Nome,Tipo,Execucao_s,Deadline_s,Status\n");

    for (int i = 0; i < NUM_THREADS; i++) {
        double start = task_queue[i].start_time.tv_sec + task_queue[i].start_time.tv_nsec / 1e9;
        double finish = task_queue[i].finish_time.tv_sec + task_queue[i].finish_time.tv_nsec / 1e9;
        double exec_time = finish - start;

        long deadline_sec = task_queue[i].deadline.tv_sec;
        const char* tipo = (task_queue[i].type == PERIODIC) ? "Periodica" : "Aperiodica";
        const char* status = task_queue[i].missed_deadline ? "Violado" : "OK";

        fprintf(fp, "%d,%s,%s,%.6f,%ld,%s\n",
                task_queue[i].id,
                task_queue[i].name,
                tipo,
                exec_time,
                deadline_sec,
                status);
    }

    fclose(fp);
    printf("📄 Relatório salvo em: %s\n", filename);
}