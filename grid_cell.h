#ifndef GRID_CELL_H_
#define GRID_CELL_H_
#include "defs.h"
// Grid dimensions for spatial partitioning
#define GRID_SIZE_X 32
#define GRID_SIZE_Y 32
#define GRID_CELL_SIZE (VISIBLE_RANGE * 1.5f)
#define MAX_BOIDS_PER_CELL 32

struct GridCell {
    int boid_indices[MAX_BOIDS_PER_CELL];
    int count;
};

#endif // GRID_CELL_H_
