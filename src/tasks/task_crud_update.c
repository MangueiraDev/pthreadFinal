// ==========================
// FILE: src/tasks/task_crud_update.c
// ==========================
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "db.h"
#include "task.h"

void* task_crud_update(void* arg) {
    printf("[UPDATE] Iniciando atualização\n");
    // Gerar nome e email aleatórios para o usuário
    char nomes[][10] = {"Ana", "Bruno", "Carlos", "Diana", "Eduardo", "Fernanda", "Gabriel", "Helena"};
    int idx = rand() % (sizeof(nomes)/sizeof(nomes[0]));
    char email[50];
    snprintf(email, sizeof(email), "%s%d@example.com", nomes[idx], rand() % 1000);

    atualiza_usuario(1, nomes[idx], email);
    sleep(1); // simula tempo de processamento
    printf("[UPDATE] Finalizada atualização\n");
    return NULL;
}