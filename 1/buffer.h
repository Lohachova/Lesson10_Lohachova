#ifndef OPERATING_SYSTEM_BUFFER_H
#define OPERATING_SYSTEM_BUFFER_H

#include <semaphore.h>

typedef struct {
    int *buffer;
    int size;
    int ind;
    sem_t lock; /* Semaphore for critical section */
    sem_t empty_items; /* Semaphore for empty buffer. */
    sem_t full_items; /* Semaphore for full buffer */
} BUFFER;

void init(BUFFER * buf, int size);
void destroy(BUFFER * buf);
void set(BUFFER * buf, int elem);
int get(BUFFER * buf);

#endif //OPERATING_SYSTEM_BUFFER_H

