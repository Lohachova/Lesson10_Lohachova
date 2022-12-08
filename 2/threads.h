#ifndef OPERATING_SYSTEM_THREADS_H
#define OPERATING_SYSTEM_THREADS_H

#include "buffer.h"

typedef struct {
    int num_thread;
    int num_reps;
    BUFFER * buf;
} TARG;

void *detach_thread(void * arg);
void *producer_thread(void * arg);
void *consumer_thread(void * arg);

#endif //OPERATING_SYSTEM_THREADS_H

