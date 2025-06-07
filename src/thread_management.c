#include "../headers/thread_management.h"
#include "../headers/behavior_bias.h"
#include "../headers/big_check_update.h"
#include "../headers/boid_core.h"
#include "../headers/defs.h"
#include "../headers/small_check_update.h"

#include <stdio.h>
#include <stdlib.h>

static threadStruct *threads = NULL;
static int thread_count = 0;

void generate_thread_objects(Boid *boid_array) {
    int boid_count = BOID_COUNT;
    int base_chunk_size, remainder;

    if (boid_count <= MAX_CHUNK_SIZE * MAX_THREADS) {
        thread_count = (boid_count + MAX_CHUNK_SIZE - 1) / MAX_CHUNK_SIZE;
        base_chunk_size = MAX_CHUNK_SIZE;
        remainder = boid_count % base_chunk_size;
    } else {
        thread_count = MAX_THREADS;
        base_chunk_size = boid_count / thread_count;
        remainder = boid_count % thread_count;
    }

    // INIT and populate the structs in the array
    threads = malloc(sizeof(threadStruct) * thread_count);

    int start = 0;

    for (int i = 0; i < thread_count; i++) {
        int chunk_size = base_chunk_size;

        if (boid_count > MAX_CHUNK_SIZE * MAX_THREADS) {
            if (i < remainder)
                chunk_size += 1;
        } else {
            if (i == thread_count - 1)
                chunk_size += remainder;
        }
        threads[i].chunk.chunk_start = start;
        threads[i].chunk.chunk_end = start + chunk_size;
        if (threads[i].chunk.chunk_end > boid_count) {
            threads[i].chunk.chunk_end = boid_count;
        }

        threads[i].array = boid_array;

        start += chunk_size;
    }
    fprintf(stdout, "Thread generation on chunk successful\n");
}

// Go through and change all of the function alls for the main loop.
static void *core_sim_thrfn(void *args) {
    threadStruct *thread_array = (threadStruct *)args;

    int start = thread_array->chunk.chunk_start;
    int end = thread_array->chunk.chunk_end;

    boid_collision(thread_array->array, start, end);
    boid_cohesion(thread_array->array, start, end);
    apply_bias(thread_array->array, start, end);
    limit_speed(thread_array->array, start, end);
    update_position(thread_array->array, start, end);
    check_bounds(thread_array->array, 50, SCREEN_WIDTH - 50, 50,
                 SCREEN_HEIGHT - 50, start, end);

    return NULL;
}

void run_threads() {

    if (!threads || thread_count == 0) {
        fprintf(stderr, "Error: Threads not initialised\n");
    }

    for (int i = 0; i < thread_count; i++) {
        pthread_create(&threads[i].threadID, NULL, core_sim_thrfn, &threads[i]);
    }
}

void end_threads() {
    for (int i = 0; i < thread_count; i++) {
        pthread_join(threads[i].threadID, NULL);
    }
}

void cleanup_threads() {
    free(threads);
    threads = NULL;
    thread_count = 0;
}
