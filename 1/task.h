#ifndef OPERATING_SYSTEM_TASK_H
#define OPERATING_SYSTEM_TASK_H

#include "buffer.h"

typedef struct {
    int num_readers;
    int num_writers;
    int work_time;
    BUFFER * buf;
} TPARAM;

void task_solution(TPARAM * init);

#endif //OPERATING_SYSTEM_TASK_H

