#include <pthread.h>

#include "../headers/defs.h"
#include "../headers/thread_management.h"

static void calculate_threads(int *thread_count) {

    if (BOID_COUNT <= 500) {
        *thread_count = 1;
    }
}

void init_threads() {

    // Used to store the number of threads and chunk structs as ea thread will
    // own one chunk.
    int thread_count;

    calculate_threads(&thread_count);

    if (!thread_count) {
        return;
    }

    boidChunk chunk_array[thread_count];

    for (int i = 0; i < thread_count; i++) {
    }
}
