#ifndef THREAD_MANAGEMENT_H
#define THREAD_MANAGEMENT_H

#include "boid_core.h"

typedef struct {
    Boid *boids;
    int chunkStart;
    int chunkEnd;
} boidChunk;

void init_threads();

void end_threads();

void run_threads();

#endif
