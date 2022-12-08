#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"
#include "task.h"

int main() {
    TPARAM init;
    BUFFER buf;
    initializer(&buf, 7);

    init.buf = &buf;
    init.num_readers = 5;
    init.num_writers = 6;
    init.num_reps = 100;
    init.work_time = 5;
    task_solution(&init);

    destructor(&buf);

    printf("\nFinish\n");

    return EXIT_SUCCESS;
}

