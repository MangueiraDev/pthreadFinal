# Realtime Thread Scheduler in C (Round Robin & FIFO)

## Objetivo do Projeto
Simular um **sistema de escalonamento de tarefas em tempo real** utilizando threads POSIX (PThreads) em C. 

## Estrutura:
pthreadsFinal/
├── include/              # Arquivos de cabeçalho (.h)
├── src/                  # Implementações em C (.c)
├── build/                # Binário gerado após compilação
├── Makefile              # Script de compilação
├── .gitignore            # Arquivos ignorados no Git
└── README.md             # Este arquivo

---
## Implementa dois algoritmos de escalonamento:

### 📥 FIFO - First-In, First-Out (Tarefas são executadas na ordem de chegada, sem preempção)
Cada thread aguarda seu turno com base no `current_turn_id`. 
A thread só avança quando for a próxima da fila, realizando seu trabalho completo de uma vez.

### 🔁 Round Robin (Tarefas recebem um quantum fixo de tempo)

Cada tarefa possui:
- `id`: identificador
- `total_work_units`: número total de unidades de trabalho (entre 3 e 8)
- `progress`: unidades já concluídas

O escalonador (`scheduler.c`) dá turnos de tempo fixo (`TIME_QUANTUM`) para cada tarefa. 
    Se a tarefa não for concluída nesse tempo, ela será pausada e retomada no próximo ciclo.

As threads usam **condições (pthread_cond_t)** para sincronizar execução com o escalonador, respeitando o turno.

---
## Componentes do Código:

### main.c
Função principal do sistema
• Define o tipo de escalonamento: Round Robin (0) ou FIFO (1)
• Inicializa tarefas e threads
• Cria variáveis de sincronização (mutexes e condições)
• Aguarda finalização das threads com pthread_join
• Libera recursos

## task.c
Lógica do trabalho das tarefas
• do_work_rr(): executa a tarefa parcialmente (quantum), usada no Round Robin
• do_work_fifo(): executa a tarefa completamente, usada no FIFO
• Simula execução com sleep e atualiza progresso
• Exibe mensagens indicando progresso e término

## thread_funcs.c
Funções executadas pelas threads

### round_robin_thread_func():
• Aguarda sinal do escalonador
• Executa parte da tarefa (respeitando quantum)
• Informa conclusão ou pausa

### fifo_thread_func():
• Aguarda sua vez na fila
• Executa a tarefa inteira de uma vez
• Sinaliza a próxima thread

## scheduler.c
Escalonador (FIFO ou Round Robin)
### Quando for FIFO:
A lógica de escalonamento está dentro da função fifo_thread_func(), que:
• Controla qual thread executa com base em current_turn_id
• Aguarda sua vez com pthread_cond_wait()
• Executa a tarefa por completo (sem interrupções)
• Sinaliza a próxima thread com pthread_cond_signal()

### Quando for Round Robin:
A lógica de escalonamento está dentro da função run_round_robin_scheduler(), que:
• Controla qual thread executa a cada quantum
• Sinaliza a thread da vez com pthread_cond_signal()
• Aguarda a conclusão do quantum com pthread_cond_wait()
• Gira entre as threads até todas finalizarem

## task.h
Declaração da estrutura e funções de tarefa
• Define a struct Task { id, total_work_units, progress }
• Declara funções do_work_rr() e do_work_fifo()
• Declara variáveis globais compartilhadas com threads/escalonador

## thread_funcs.h
Assinaturas das funções de thread
• Declara round_robin_thread_func(void*)
• Declara fifo_thread_func(void*)

## scheduler.h
Cabeçalho do escalonador (FIFO ou Round Robin)

### Quando for FIFO:
O escalonamento é feito dentro da função fifo_thread_func(), que:
• Controla qual thread executa com base em current_turn_id
• Aguarda sua vez com pthread_cond_wait()
• Executa a tarefa por completo (sem preempção)
• Sinaliza a próxima thread com pthread_cond_signal()

### Quando for Round Robin:
Declara a função run_round_robin_scheduler(), que:
• Controla qual thread executa a cada quantum
• Sinaliza a thread com pthread_cond_signal()
• Aguarda a thread com pthread_cond_wait()
• Gira entre as threads até todas finalizarem

## Makefile
Script de compilação automatizado
• Compila todos os arquivos .c da pasta src/
• Gera binário final no diretório build/

## .gitignore
Ignora arquivos desnecessários no Git
• build/: pasta de binários
• *.o: arquivos objeto intermediários
• .vscode/ e .DS_Store: arquivos de ambiente/editor

---
## Executar
make clean
make fifo && ./build/scheduler_fifo
make rr && ./build/scheduler_rr