#include <pthread.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "threads.h"
#include "buffer.h"

#define MAX 10

void *producer(void *arg) {
    int item;
    int old_cancel_state;
    BUFFER * buf = (BUFFER *) arg;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    srand(time(NULL));
    while(1) {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
        item = rand() % MAX; /* make object to store */
        set(buf, item);
        pthread_setcancelstate(old_cancel_state, NULL);
        pthread_testcancel();
        printf("Producer(%u): = %d\n", (unsigned int)pthread_self(), item);
        sleep(1);
    }
    return NULL;
}

void *consumer(void *arg) {
    int item;
    int old_cancel_state;
    BUFFER * buf = (BUFFER *) arg;

    pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    while(1) {
        pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state);
        item = get(buf);
        pthread_setcancelstate(old_cancel_state, NULL);
        pthread_testcancel();
        printf("\tConsumer(%u): = %d\n", (unsigned int)pthread_self(), item);
        sleep(1);
    }
    return NULL;
}

