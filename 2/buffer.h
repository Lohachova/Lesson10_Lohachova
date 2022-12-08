#ifndef OPERATING_SYSTEM_BUFFER_H
#define OPERATING_SYSTEM_BUFFER_H

#include <stdio.h>
#include <pthread.h>

typedef struct {
    double *buffer;
    int size;
    int ind;
    pthread_mutex_t lock;
    pthread_cond_t empty;
    pthread_cond_t fill;
} BUFFER;

void initializer(BUFFER * buf, int num);
void destructor(BUFFER * buf);
void show(BUFFER * buf, FILE ** fptr);
void put(BUFFER * buf, double value);
double get(BUFFER * buf);

#endif //OPERATING_SYSTEM_BUFFER_H

