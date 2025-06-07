#ifndef THREAD_MANAGEMENT_H
#define THREAD_MANAGEMENT_H

#include <pthread.h>

#include "boid_core.h"

// The chunk that one thread works on.
typedef struct {
    int chunk_start;
    int chunk_end;

} boidChunk;

typedef struct {
    // Chunk struct
    boidChunk chunk;
    // array to work on
    Boid *array;
    // Thread ID
    pthread_t threadID;

} threadStruct;

void generate_thread_objects(Boid *boid_array);

void run_threads();

void end_threads();

void cleanup_threads();

#endif
