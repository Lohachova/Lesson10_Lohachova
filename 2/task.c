#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "task.h"
#include "threads.h"

typedef struct {
    pthread_t * thread;
    TARG * arg;
} MAKE_RES;

static pthread_t make_detached(BUFFER * buf);
static MAKE_RES * make_producers(TARG * targ);
static MAKE_RES * make_consumers(TARG * targ);

void task_solution(TPARAM * init) {
    pthread_t thread;
    TARG arg_producers, arg_consumers;
    MAKE_RES * producers;
    MAKE_RES * consumers;
    int i;

    thread = make_detached(init->buf);
    arg_producers.buf = init->buf;
    arg_producers.num_reps = init->num_reps;
    arg_producers.num_thread = init->num_writers;
    producers = make_producers(&arg_producers);

    arg_consumers.buf = init->buf;
    arg_consumers.num_reps = init->num_reps;
    arg_consumers.num_thread = init->num_readers;
    consumers = make_consumers(&arg_consumers);

    sleep(init->work_time);
    for (i = 0; i < init->num_writers; i++) {
        if (pthread_cancel(producers->thread[i]) != 0) {
            fprintf(stderr, "Writer Thread %d Canceling Error\n", i);
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < init->num_readers; i++) {
        if (pthread_cancel(consumers->thread[i]) != 0) {
            fprintf(stderr, "Reader Thread %d Canceling Error\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < init->num_writers; i++) {
        if (pthread_join(producers->thread[i], NULL) != 0) {
            fprintf(stderr, "Writer Thread %d Waiting Error\n", i);
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < init->num_readers; i++) {
        if (pthread_join(consumers->thread[i], NULL) != 0) {
            fprintf(stderr, "Reader Thread %d Waiting Error\n", i);
            exit(EXIT_FAILURE);
        }
    }

    free(producers->thread);  free(producers->arg);  free(producers);
    free(consumers->thread);  free(consumers->arg);  free(consumers);

    if(pthread_cancel(thread)) {
        fprintf(stderr, "ERROR! Cannot stop detached thread!\n");
        exit(EXIT_FAILURE);
    }
}

static pthread_t make_detached(BUFFER * buf) {
    pthread_attr_t attr;
    pthread_t thread;

    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    if (pthread_create(&thread, &attr, &detach_thread, (void*)buf)) {
        fprintf(stderr, "Error while Creation Detached Thread\n");
        exit(EXIT_FAILURE);
    }
    pthread_attr_destroy(&attr);

    return thread;
}


static MAKE_RES * make_producers(TARG * targ) {
    MAKE_RES * res;
    TARG * arg;
    int i;

    res = (MAKE_RES *) malloc(sizeof(MAKE_RES));
    if (!res) {
        fprintf(stderr, "Allocation memory error\n");
        exit(EXIT_FAILURE);
    }
    res->thread = (pthread_t *) calloc(targ->num_thread, sizeof(pthread_t));
    res->arg = (TARG *) calloc(targ->num_thread, sizeof(TARG));
    if ((res->thread == NULL) || (res->arg == NULL)) {
        fprintf(stderr, "Allocation memory error\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < targ->num_thread; i++) {
        res->arg[i].num_reps = targ->num_reps;
        res->arg[i].num_thread = i;
        res->arg[i].buf = targ->buf;
        if (pthread_create(&(res->thread[i]), NULL, &producer_thread, &(res->arg[i]))) {
            fprintf(stderr, "Writer Thread %d Creation Error\n", i);
            exit(EXIT_FAILURE);
        }
    }

    return res;
}


static MAKE_RES * make_consumers(TARG * targ) {
    MAKE_RES * res;
    TARG * arg;
    int i;

    res = (MAKE_RES *) malloc(sizeof(MAKE_RES));
    if (!res) {
        fprintf(stderr, "Allocation memory error\n");
        exit(EXIT_FAILURE);
    }
    res->thread = (pthread_t *) calloc(targ->num_thread, sizeof(pthread_t));
    res->arg = (TARG *) calloc(targ->num_thread, sizeof(TARG));
    if ((res->thread == NULL) || (res->arg == NULL)) {
        fprintf(stderr, "Allocation memory error\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < targ->num_thread; i++) {
        res->arg[i].num_reps = targ->num_reps;
        res->arg[i].num_thread = i;
        res->arg[i].buf = targ->buf;
        if (pthread_create(&(res->thread[i]), NULL, &consumer_thread, &(res->arg[i]))) {
            fprintf(stderr, "Reader Thread %d Creation Error\n", i);
            exit(EXIT_FAILURE);
        }
    }

    return res;
}

