// ==========================
// FILE: include/scheduler.h
// ==========================
#ifndef SCHEDULER_H
#define SCHEDULER_H

#ifdef FIFO_MODE
void run_fifo_scheduler();
#else
void run_round_robin_scheduler();
#endif

#endif
