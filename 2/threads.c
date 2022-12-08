#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "threads.h"
#include "buffer.h"

void * detach_thread(void * arg) {
    BUFFER buf = *(BUFFER*)arg;

    if (pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL)) {
        fprintf(stderr, "Cannot change cancel state\n");
        exit(EXIT_FAILURE);
    }
    if (pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL)) {
        fprintf(stderr, "Cannot change cancel type\n");
        exit(EXIT_FAILURE);
    }
    //printf("\tDetached Thread is Started\n");
    while (1) {
        //printf("Current state:");
        show(&buf, &stdout);
        //printf("\n");
        sleep(1);
    }
    //printf("\tDetached Thread is Stopped\n");
    return NULL;
}


void * producer_thread(void * arg) {
    TARG * targ = (TARG *)arg;
    int k, ind;
    double val;
    printf("\tWriter Thread %d is started\n", targ->num_thread);
    sleep(targ->num_thread);
    srand(time(NULL));
    for (k = 0; k < targ->num_reps; k++) {
        val = (double)rand() / RAND_MAX;
        put(targ->buf, val);
        //printf("\tWriter Thread No %d, res[%d] = %g\n", targ->num_thread, ind, val);
        printf("\tWriter Thread No %d, res = %g\n", targ->num_thread, val);
        sleep(rand() % targ->buf->size);
    }
    printf("\tWriter Thread %d is stopped\n", targ->num_thread);
    return NULL;
}


void * consumer_thread(void * arg) {
    TARG * targ = (TARG *)arg;
    int k, ind;
    double res;
    printf("\t\tReader Thread %d is started\n", targ->num_thread);
    sleep(targ->num_thread);
    srand(time(NULL));
    for (k = 0; k < targ->num_reps; k++) {
        res = get(targ->buf);
        printf("\t\tReader Thread No %d, res = %g\n", targ->num_thread, res);
        //printf("\t\tReader Thread No %d, res[%d] = %g\n", targ->num_thread, ind, res);
        sleep(rand() % targ->buf->size);
    }
    printf("\t\tReader Thread %d is stopped\n", targ->num_thread);
    return NULL;
}

