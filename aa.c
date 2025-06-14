#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#define NUM_THREADS 5
#define TIME_QUANTUM 2 // Quantum de 3 unidades de trabalho (segundos)

// --- Estrutura da Tarefa com Progresso ---
typedef struct {
    int id;
    int total_work_units; // O tamanho total da tarefa
    int progress;         // Quantas unidades já foram concluídas
} Task;

Task task_queue[NUM_THREADS];
int tasks_completed_count = 0;

// Mutex e variáveis de condição
bool task_done[NUM_THREADS];

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_scheduler_wait;
pthread_cond_t cond_thread_work[NUM_THREADS];

int current_turn_id = 0;

/**
 * @brief Executa o trabalho da tarefa, unidade por unidade, de forma pausável.
 * @param task Ponteiro para a tarefa a ser processada.
 * @param units_to_do Número de unidades de trabalho a executar neste quantum.
 */
void do_work(Task* task, int units_to_do) {
    printf("Thread %d: Retomando trabalho. Progresso atual: %d/%d.\n",
           task->id, task->progress, task->total_work_units);

    int units_done_this_turn = 0;
    for (int i = 0; i < units_to_do; ++i) {
        // Verifica se a tarefa já terminou
        if (task->progress >= task->total_work_units) {
            break;
        }

        // Simula uma unidade de trabalho
        sleep(1);
        task->progress++;
        units_done_this_turn++;
        printf("  -> Thread %d: Progresso: %d/%d\n", task->id, task->progress, task->total_work_units);
    }
    printf("Thread %d: Trabalhou por %d unidades neste quantum.\n", task->id, units_done_this_turn);
}


/**
 * @brief Função da thread de trabalho.
 */
void* round_robin_thread_func(void* arg) {
    int my_id = *(int*)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    while (true) {
        pthread_cond_wait(&cond_thread_work[my_id], &mutex);

        // Se a tarefa já está completa, apenas sai.
        if (task_queue[my_id].progress >= task_queue[my_id].total_work_units) {
            break;
        }

        // --- SEÇÃO CRÍTICA ---
        printf("\n=> Thread %d: Assumindo CPU. Quantum: %d unidades.\n", my_id, TIME_QUANTUM);

        // Executa o trabalho, que agora modifica o progresso internamente
        do_work(&task_queue[my_id], TIME_QUANTUM);

        // Verifica se a tarefa foi concluída NESTE TURNO
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

/**
 * @brief Escalonador Round Robin (executado na thread main).
 */
void run_round_robin_scheduler() {
    sleep(1);
    pthread_mutex_lock(&mutex);

    while (tasks_completed_count < NUM_THREADS) {
        Task* current_task = &task_queue[current_turn_id];

        // Se a tarefa da vez ainda não terminou, executa.
        if (current_task->progress < current_task->total_work_units) {
            pthread_cond_signal(&cond_thread_work[current_turn_id]);
            pthread_cond_wait(&cond_scheduler_wait, &mutex);
        }

        // Passa o turno para o próximo da fila
        current_turn_id = (current_turn_id + 1) % NUM_THREADS;
    }

    // Despertar final para todas as threads terminarem
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_signal(&cond_thread_work[i]);
    }
    pthread_mutex_unlock(&mutex);
    printf("\n--- ESCALONADOR: Todas as tarefas foram concluídas! Encerrando. ---\n");
}



/**
 * @brief Função que simula o trabalho de uma thread.
 * A thread imprime seu ID e simula um tempo de processamento.
 * @param thread_id O ID da thread.
 */
void do_work_fifo(int thread_id) {
	printf("Thread %d: Iniciando trabalho...\n", thread_id);
	int work_time = thread_id + (rand() % 6); // Gera o tempo de trabalho individualmente
	sleep(work_time); // Simula o trabalho

	printf("Thread %d: Trabalho concluído. Tempo de processamento: %d segundos.\n", thread_id, work_time);
}

/**
 * @brief Função executada por cada thread para o escalonamento FIFO.
 * As threads esperam pela sua vez de acordo com a ordem de chegada.
 */
void* fifo_thread_func(void* arg) {
    int thread_id = *(int*)arg;

    // Bloqueia o mutex para esperar pela sua vez
    pthread_mutex_lock(&mutex);
    while (thread_id != current_turn_id) {
        pthread_cond_wait(&cond_thread_work[thread_id], &mutex);
    }

    // Seção Crítica
    printf("Thread %d: Entrou na seção crítica.\n", thread_id);
    do_work_fifo(thread_id);
    printf("Thread %d: Saindo da seção crítica.\n\n", thread_id);

    // Próxima thread pode executar
    current_turn_id++;

    if (current_turn_id < NUM_THREADS) {
        pthread_cond_signal(&cond_thread_work[current_turn_id]);
    }
    
    pthread_mutex_unlock(&mutex);
    
    free(arg);
    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    srand(time(NULL));

	int escalonamento = 0; // Variável para armazenar o tipo de escalonamento (0 = Round Robin, 1 = Fifo, etc.)

	if (escalonamento == 0)
	{
		 printf("--- Simulação Round Robin com Trabalho Pausável (Quantum = %d) ---\n", TIME_QUANTUM);

		for (int i = 0; i < NUM_THREADS; i++) {
			task_queue[i].id = i;
			task_queue[i].total_work_units = 3 + (rand() % 6); // Tarefas de 3 a 8 unidades
			task_queue[i].progress = 0; // Inicia com progresso 0
			printf("  - Tarefa %d criada com %d unidades de trabalho.\n", i, task_queue[i].total_work_units);
		}

		printf("\n");

		pthread_cond_init(&cond_scheduler_wait, NULL);

		for (int i = 0; i < NUM_THREADS; i++) {
			
			pthread_cond_init(&cond_thread_work[i], NULL);
			int* thread_id = malloc(sizeof(int));
			*thread_id = i;

			pthread_create(&threads[i], NULL, round_robin_thread_func, thread_id);
		}

		run_round_robin_scheduler();

	}else if (escalonamento == 1){

		// Inicializa variáveis de condição e flags de conclusão
		for (int i = 0; i < NUM_THREADS; i++) {
			pthread_cond_init(&cond_thread_work[i], NULL);
			task_done[i] = false;
		}

		// Cria as threads para FIFO
        for (int i = 0; i < NUM_THREADS; i++) {
            int* thread_id = malloc(sizeof(int));
            *thread_id = i;
            pthread_create(&threads[i], NULL, fifo_thread_func, thread_id);
        }
        
        // Libera a primeira thread para começar
        sleep(1); // Garante que as threads estejam esperando
        pthread_cond_signal(&cond_thread_work[0]);

	}
	
   

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_scheduler_wait);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_cond_destroy(&cond_thread_work[i]);
    }

    return 0;
}