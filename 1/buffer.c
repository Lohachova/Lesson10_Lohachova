#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"

void init(BUFFER * buf, int size) {
    buf->buffer = (int *)calloc(size, sizeof(int));
    buf->ind = 0;

    if (sem_init(&(buf->lock), 0, 1) != 0) {
        fprintf(stderr, "Critical Section Semaphore Init Error\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&(buf->empty_items), 0, size) != 0) {
        fprintf(stderr, "Empty Buffer Semaphore Init Error\n");
        exit(EXIT_FAILURE);
    }
    if (sem_init(&(buf->full_items), 0, 0) != 0) {
        fprintf(stderr, "Full Buffer Semaphore Init Error\n");
        exit(EXIT_FAILURE);
    }
}

void destroy(BUFFER * buf) {
    sem_destroy(&(buf->lock));
    sem_destroy(&(buf->empty_items));
    sem_destroy(&(buf->full_items));
    free(buf->buffer);
    buf->buffer = NULL;
    buf->ind = 0;
}

void set(BUFFER * buf, int elem) {
    int pos;
    sem_wait(&(buf->empty_items)); /* Is place in buffer for object */
    sem_wait(&(buf->lock));        /* Go to critical section */
    pos = buf->ind;
    buf->buffer[pos] = elem;       /* Set Data into buffer */
    buf->ind += 1;
    sem_post(&(buf->lock));        /* Go from critical section */
    sem_post(&(buf->full_items));
}

int get(BUFFER * buf) {
    int pos, item;
    sem_wait(&(buf->full_items)); /* Is items in buffer */
    sem_wait(&(buf->lock));       /* Go to critical section */
    pos = buf->ind-1;
    item = buf->buffer[pos];      /* Take Data from buffer */
    buf->buffer[pos] = 0;
    buf->ind -= 1;
    sem_post(&(buf->lock));       /* Go from critical section */
    sem_post(&(buf->empty_items));
    return item;
}

