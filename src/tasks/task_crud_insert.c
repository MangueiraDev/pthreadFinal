// ==========================
// FILE: src/tasks/task_crud_insert.c
// ==========================
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_insert(void* arg) {
    printf("[INSERT] Iniciando tarefa de inserção\n");
    
    // Gerar nome e email aleatórios para o usuário
    char nomes[][10] = {"Ana", "Bruno", "Carlos", "Diana", "Eduardo", "Fernanda", "Gabriel", "Helena"};
    
    int idx = rand() % (sizeof(nomes)/sizeof(nomes[0]));
    char email[50];
    snprintf(email, sizeof(email), "%s%d@example.com", nomes[idx], rand() % 1000);

    insere_usuario(nomes[idx], email);
// Simular latência aleatória entre 2 e 12 segundos
    int latency = (rand() % 6) + 2; // 2 a 12 segundos
    sleep(latency);    printf("[INSERT] Finalizada inserção\n");
    return NULL;
}