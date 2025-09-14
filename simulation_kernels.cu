#include "boid_core.h"
#include "defs.h"
#include "grid_cell.h"
#include <cub/cub.cuh>
#include <cuda_runtime.h>
#include <math.h>

__constant__ float c_visible_range_squared = VISIBLE_RANGE_SQUARED;
__constant__ float c_protected_range_squared = PROTECTED_RANGE_SQUARED;
__constant__ float c_centering_factor = CENTERING_FACTOR;
__constant__ float c_avoid_factor = AVOID_FACTOR;
__constant__ float c_matching_factor = MATCHING_FACTOR;
__constant__ float c_turn_factor = TURN_FACTOR;
__constant__ float c_min_speed = MIN_SPEED;
__constant__ float c_max_speed = MAX_SPEED;
__constant__ float c_max_bias = MAX_BIAS;
__constant__ float c_bias_increment = BIAS_INCREMENT;
__constant__ float c_delta_time = DELTA_TIME;
__constant__ float c_speed_scale = SPEED_SCALE;

// Kernel to build spatial grid
__global__ void build_grid_kernel(Boid *boids, GridCell *grid, int n,
                                  float grid_min_x, float grid_min_y,
                                  float grid_cell_size, int grid_size_x) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n)
        return;

    Boid *b = &boids[i];

    // Calculate grid cell coordinates
    int cell_x = floorf((b->x_pos - grid_min_x) / grid_cell_size);
    int cell_y = floorf((b->y_pos - grid_min_y) / grid_cell_size);

    cell_x = max(0, min(grid_size_x - 1, cell_x));
    cell_y = max(0, min(GRID_SIZE_Y - 1, cell_y));

    int cell_idx = cell_y * grid_size_x + cell_x;

    // Atomically add boid to grid cell
    int pos = atomicAdd(&grid[cell_idx].count, 1);
    if (pos < MAX_BOIDS_PER_CELL) {
        grid[cell_idx].boid_indices[pos] = i;
    }
}

// Optimized collision and cohesion kernel using spatial grid
__global__ void collision_cohesion_kernel_optimized(Boid *boids, GridCell *grid,
                                                    int n, float grid_min_x,
                                                    float grid_min_y,
                                                    float grid_cell_size,
                                                    int grid_size_x) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n)
        return;

    Boid *bptri = &boids[i];

    // Initialize accumulation variables
    float xpos_avg = 0.0f;
    float ypos_avg = 0.0f;
    float xvel_avg = 0.0f;
    float yvel_avg = 0.0f;
    float close_dx = 0.0f;
    float close_dy = 0.0f;
    int neighboring_boids = 0;

    // Calculate which grid cell this boid is in
    int cell_x = floorf((bptri->x_pos - grid_min_x) / grid_cell_size);
    int cell_y = floorf((bptri->y_pos - grid_min_y) / grid_cell_size);

    // Check neighboring cells (3x3 area around the boid)
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int check_x = cell_x + dx;
            int check_y = cell_y + dy;

            if (check_x < 0 || check_x >= grid_size_x || check_y < 0 ||
                check_y >= GRID_SIZE_Y) {
                continue;
            }

            int cell_idx = check_y * grid_size_x + check_x;
            GridCell *cell = &grid[cell_idx];

            // Check all boids in this cell
            for (int j = 0; j < cell->count; j++) {
                int other_idx = cell->boid_indices[j];
                if (other_idx == i)
                    continue; // Skip self

                Boid *bptrj = &boids[other_idx];
                float dx = bptri->x_pos - bptrj->x_pos;
                float dy = bptri->y_pos - bptrj->y_pos;
                float distance_squared = dx * dx + dy * dy;

                // Collision avoidance
                if (distance_squared < c_protected_range_squared) {
                    close_dx += dx;
                    close_dy += dy;
                }

                // Cohesion and alignment
                if (distance_squared < c_visible_range_squared &&
                    distance_squared > c_protected_range_squared) {
                    xpos_avg += bptrj->x_pos;
                    ypos_avg += bptrj->y_pos;
                    xvel_avg += bptrj->x_vel;
                    yvel_avg += bptrj->y_vel;
                    neighboring_boids++;
                }
            }
        }
    }

    // Apply collision avoidance
    bptri->x_vel += close_dx * c_avoid_factor;
    bptri->y_vel += close_dy * c_avoid_factor;

    // Apply cohesion and alignment
    if (neighboring_boids > 0) {
        xpos_avg /= neighboring_boids;
        ypos_avg /= neighboring_boids;
        xvel_avg /= neighboring_boids;
        yvel_avg /= neighboring_boids;

        bptri->x_vel += (xpos_avg - bptri->x_pos) * c_centering_factor;
        bptri->y_vel += (ypos_avg - bptri->y_pos) * c_centering_factor;

        bptri->x_vel += (xvel_avg - bptri->x_vel) * c_matching_factor;
        bptri->y_vel += (yvel_avg - bptri->y_vel) * c_matching_factor;
    }
}

// Kernel for applying bias
__global__ void apply_bias_kernel(Boid *boids, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n)
        return;

    Boid *bptr = &boids[i];

    if (bptr->is_scout_group_1) {
        if (bptr->x_vel > 0)
            bptr->biasval = fminf(c_max_bias, bptr->biasval + c_bias_increment);
        else
            bptr->biasval =
                fmaxf(c_bias_increment, bptr->biasval - c_bias_increment);

        bptr->x_vel =
            (1.0f - bptr->biasval) * bptr->x_vel + bptr->biasval * 1.0f;
    } else {
        if (bptr->x_vel < 0)
            bptr->biasval = fminf(c_max_bias, bptr->biasval + c_bias_increment);
        else
            bptr->biasval =
                fmaxf(c_bias_increment, bptr->biasval - c_bias_increment);

        bptr->x_vel =
            (1.0f - bptr->biasval) * bptr->x_vel - bptr->biasval * 1.0f;
    }
}

// Kernel for speed limiting
__global__ void limit_speed_kernel(Boid *boids, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n)
        return;

    Boid *bptr = &boids[i];
    float speed = sqrtf(bptr->x_vel * bptr->x_vel + bptr->y_vel * bptr->y_vel);

    if (speed < c_min_speed) {
        bptr->x_vel = (bptr->x_vel / speed) * c_min_speed;
        bptr->y_vel = (bptr->y_vel / speed) * c_min_speed;
    }
    if (speed > c_max_speed) {
        bptr->x_vel = (bptr->x_vel / speed) * c_max_speed;
        bptr->y_vel = (bptr->y_vel / speed) * c_max_speed;
    }
}

// Kernel for updating positions
__global__ void update_position_kernel(Boid *boids, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n)
        return;

    Boid *bptr = &boids[i];
    bptr->x_pos += bptr->x_vel * c_delta_time * c_speed_scale;
    bptr->y_pos += bptr->y_vel * c_delta_time * c_speed_scale;
}

// Kernel for boundary checking
__global__ void check_bounds_kernel(Boid *boids, int n, int left_margin,
                                    int right_margin, int top_margin,
                                    int bottom_margin) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i >= n)
        return;

    Boid *bptr = &boids[i];

    if (bptr->x_pos < left_margin)
        bptr->x_vel += c_turn_factor;

    if (bptr->x_pos > right_margin)
        bptr->x_vel -= c_turn_factor;

    if (bptr->y_pos > bottom_margin)
        bptr->y_vel -= c_turn_factor;

    if (bptr->y_pos < top_margin)
        bptr->y_vel += c_turn_factor;
}
