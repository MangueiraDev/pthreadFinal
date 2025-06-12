// ==========================
// FILE: include/scheduler.h
// ==========================
#ifndef SCHEDULER_H
#define SCHEDULER_H

// Define a função pública que será chamada no main.c,
// dependendo do modo de escalonamento definido na compilação.
#ifdef FIFO_MODE
void run_fifo_scheduler();
#else
void run_round_robin_scheduler();
#endif

#endif // SCHEDULER_H