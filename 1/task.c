#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "task.h"
#include "threads.h"

void task_solution(TPARAM *init) {
    pthread_t *producers;
    pthread_t *consumers;
    pthread_attr_t attr;
    int i;

    pthread_attr_init(&attr);
    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    producers = (pthread_t *) calloc(init->num_writers, sizeof(pthread_t));
    consumers = (pthread_t *) calloc(init->num_readers, sizeof(pthread_t));

    for (i = 0; i < init->num_writers; i++) {
        if (pthread_create(&producers[i], &attr, producer, init->buf)) {
            fprintf(stderr, "Writer Thread %d Creation Error\n", i);
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < init->num_readers; i++) {
        if (pthread_create(&consumers[i], &attr, consumer, init->buf)) {
            fprintf(stderr, "Reader Thread %d Creation Error\n", i);
            exit(EXIT_FAILURE);
        }
    }
    pthread_attr_destroy(&attr);
    sleep(init->work_time);

    for (i = 0; i < init->num_readers; i++) {
        if (pthread_cancel(consumers[i]) != 0) {
            fprintf(stderr, "Reader Thread %d Canceling Error\n", i);
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < init->num_readers; i++) {
        if (pthread_join(consumers[i], NULL) != 0) {
            fprintf(stderr, "Reader Thread %d Waiting Error\n", i);
            exit(EXIT_FAILURE);
        }
    }
    printf("\t\tThe Reader Thread has been Stopped!!\n");
    for (i = 0; i < init->num_writers; i++) {
        if (pthread_cancel(producers[i]) != 0) {
            fprintf(stderr, "Writer Thread %d Canceling Error\n", i);
            exit(EXIT_FAILURE);
        }
    }
    for (i = 0; i < init->num_writers; i++) {
        if (pthread_join(producers[i], NULL) != 0) {
            fprintf(stderr, "Writer Thread %d Waiting Error\n", i);
            exit(EXIT_FAILURE);
        }
    }
    printf("\t\tThe Writer Thread has been Stopped!!\n");

    free(producers);
    free(consumers);
}

