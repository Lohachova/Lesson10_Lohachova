#include <stdlib.h>

#include "buffer.h"
#include "task.h"

int main() {
    TPARAM var;
    BUFFER buf;

    init(&buf, 7);

    var.buf = &buf;
    var.num_readers = 2;
    var.num_writers = 2;
    var.work_time = 5;
    task_solution(&var);

    destroy(&buf);

    return EXIT_SUCCESS;
}


