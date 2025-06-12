// ==========================
// FILE: include/thread_funcs.h
// ==========================
#ifndef THREAD_FUNCS_H
#define THREAD_FUNCS_H

void* round_robin_thread_func(void* arg);
void* fifo_thread_func(void* arg);

#endif
