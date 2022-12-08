#include <stdio.h>
#include <stdlib.h>
#include "buffer.h"

void initializer(BUFFER * buf, int num) {
    int res1, res2, res3;
    buf->buffer = (double *)calloc(num, sizeof(double));
    if (!buf->buffer) {
        fprintf(stderr, "ERROR!!! %s\n", "Buffer Allocation ERROR!");
        exit(EXIT_FAILURE);
    }
    buf->size = num;
    buf->ind = -1;
    res1 = pthread_mutex_init(&(buf->lock), NULL);
    res2 = pthread_cond_init(&(buf->fill), NULL);
    res3 = pthread_cond_init(&(buf->empty), NULL);
    if (res1 + res2 + res3) {
        fprintf(stderr, "ERROR!!! %s\n", "Synchronization Primitives Initialization Failed!");
        exit(EXIT_FAILURE);
    }
}

void destructor(BUFFER * buf) {
    int res1, res2, res3;
    free(buf->buffer);
    buf->buffer = NULL;
    buf->size = 0;
    buf->ind = -1;
    res1 = pthread_mutex_destroy(&(buf->lock));
    res2 = pthread_cond_destroy(&(buf->fill));
    res3 = pthread_cond_destroy(&(buf->empty));
    if (res1 + res2 + res3) {
        fprintf(stderr, "ERROR!!! %s\n", "Synchronization Primitives De-Initialization Failed!");
        exit(EXIT_FAILURE);
    }
}

void show(BUFFER * buf, FILE ** fptr) {
    pthread_mutex_lock(&(buf->lock));
    {
        int i;
        //fprintf(*fptr, "\tThread id %u is in mutex (print)\n", (unsigned int) pthread_self());
        for(i = 0; i < buf->size/*ind*/; i++) {
            fprintf(*fptr, "%9.4f", buf->buffer[i]);
        }
        printf("\n");
    }
    pthread_mutex_unlock(&(buf->lock));
}

void put(BUFFER * buf, double value) {
    pthread_mutex_lock(&(buf->lock));
    //printf("\tThread id %u is in mutex (put)\n", (unsigned int) pthread_self());
    while(buf->ind == buf->size-1) {
        pthread_cond_wait(&(buf->empty), &(buf->lock));
    }
    //printf("\tThread id %u is after cond (put)\n", (unsigned int) pthread_self());
    buf->ind += 1;
    buf->buffer[buf->ind] = value;
    //pthread_mutex_unlock(&(buf->lock)); //in 2018 change the order
    //pthread_cond_signal(&(buf->fill));  //of this lines
    pthread_cond_signal(&(buf->fill));  //This is much
    pthread_mutex_unlock(&(buf->lock)); //safer strategy
}


double get(BUFFER * buf) {
    double tmp;

    pthread_mutex_lock(&(buf->lock));
    //printf("\tThread id %u is in mutex (get)\n", (unsigned int) pthread_self());
    while(buf->ind == -1) {
        pthread_cond_wait(&(buf->fill), &(buf->lock));
    }
    //printf("\tThread id %u is after cond (get)\n", (unsigned int) pthread_self());
    tmp = buf->buffer[buf->ind];
    buf->buffer[buf->ind] = 0.0;
    buf->ind -= 1;
    //pthread_mutex_unlock(&(buf->lock)); //in 2018 change the order
    //pthread_cond_signal(&(buf->empty)); //of this lines
    pthread_cond_signal(&(buf->empty)); //This is much
    pthread_mutex_unlock(&(buf->lock)); //safer strategy

    return tmp;
}

