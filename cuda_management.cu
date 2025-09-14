#include "boid_core.h"
#include "defs.h"
#include "grid_cell.h"
#include <cuda_runtime.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

extern "C" {
#include <raylib.h>
}

// Kernel declarations
extern __global__ void build_grid_kernel(Boid *boids, GridCell *grid, int n,
                                         float grid_min_x, float grid_min_y,
                                         float grid_cell_size, int grid_size_x);
extern __global__ void
collision_cohesion_kernel_optimized(Boid *boids, GridCell *grid, int n,
                                    float grid_min_x, float grid_min_y,
                                    float grid_cell_size, int grid_size_x);
extern __global__ void apply_bias_kernel(Boid *boids, int n);
extern __global__ void limit_speed_kernel(Boid *boids, int n);
extern __global__ void update_position_kernel(Boid *boids, int n);
extern __global__ void check_bounds_kernel(Boid *boids, int n, int left_margin,
                                           int right_margin, int top_margin,
                                           int bottom_margin);

// Grid parameters
const float GRID_MIN_X = -150.0f;
const float GRID_MIN_Y = 90.0f;
const int GRID_CELL_COUNT = GRID_SIZE_X * GRID_SIZE_Y;

Boid *createBoidArrayCUDA() {
    Boid *d_boids;
    cudaError_t err = cudaMalloc(&d_boids, BOID_COUNT * sizeof(Boid));
    if (err != cudaSuccess) {
        printf("CUDA malloc error: %s\n", cudaGetErrorString(err));
        return NULL;
    }

    // Create temporary host array to initialize device memory
    Boid *h_boids = (Boid *)malloc(BOID_COUNT * sizeof(Boid));
    if (!h_boids) {
        printf("Host boid array creation unsuccessful\n");
        cudaFree(d_boids);
        return NULL;
    }

    for (int i = 0; i < BOID_COUNT; i++) {
        Boid *bptr = &h_boids[i];

        // Initialize values
        float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * PI;
        float speed = MIN_SPEED + ((float)rand() / (float)RAND_MAX) *
                                      (MAX_SPEED - MIN_SPEED);

        bptr->x_pos = (float)GetRandomValue(0, SCREEN_WIDTH);
        bptr->y_pos = (float)GetRandomValue(0, SCREEN_HEIGHT);
        bptr->x_vel = cosf(angle) * speed;
        bptr->y_vel = sinf(angle) * speed;
        bptr->close_dx = 0;
        bptr->close_dy = 0;
        bptr->xpos_avg = 0;
        bptr->ypos_avg = 0;
        bptr->neighboring_boids = 0;
        bptr->biasval = DEFAULT_BIAS;
        bptr->is_scout_group_1 = GetRandomValue(0, 1);
        bptr->z_pos = GetRandomValue(-5, 5);
        bptr->type = BOID_TYPE_BASIC; // Default type
    }

    // Copy initialized data to device
    err = cudaMemcpy(d_boids, h_boids, BOID_COUNT * sizeof(Boid),
                     cudaMemcpyHostToDevice);
    if (err != cudaSuccess) {
        printf("CUDA memcpy error: %s\n", cudaGetErrorString(err));
        cudaFree(d_boids);
        free(h_boids);
        return NULL;
    }

    free(h_boids);

    printf("CUDA boid array creation successful\n");
    return d_boids;
}

void freeBoidArrayCUDA(Boid *d_boids) {
    cudaFree(d_boids);
    fprintf(stdout, "CUDA boid array freed\n");
}

void runCUDAKernels(Boid *d_boids) {
    int threadsPerBlock = 256;
    int blocksPerGrid = (BOID_COUNT + threadsPerBlock - 1) / threadsPerBlock;

    // Allocate grid memory
    GridCell *d_grid;
    cudaMalloc(&d_grid, GRID_CELL_COUNT * sizeof(GridCell));

    // Clear grid
    cudaMemset(d_grid, 0, GRID_CELL_COUNT * sizeof(GridCell));

    // Build spatial grid
    build_grid_kernel<<<blocksPerGrid, threadsPerBlock>>>(
        d_boids, d_grid, BOID_COUNT, GRID_MIN_X, GRID_MIN_Y, GRID_CELL_SIZE,
        GRID_SIZE_X);

    // Run optimized collision/cohesion kernel
    collision_cohesion_kernel_optimized<<<blocksPerGrid, threadsPerBlock>>>(
        d_boids, d_grid, BOID_COUNT, GRID_MIN_X, GRID_MIN_Y, GRID_CELL_SIZE,
        GRID_SIZE_X);

    // Run other kernels
    apply_bias_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_boids, BOID_COUNT);
    limit_speed_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_boids, BOID_COUNT);
    update_position_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_boids,
                                                               BOID_COUNT);
    check_bounds_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_boids, BOID_COUNT,
                                                            -150, 150, 90, 260);

    // Free grid memory
    cudaFree(d_grid);

    // Synchronize to check for errors
    cudaError_t err = cudaDeviceSynchronize();
    if (err != cudaSuccess) {
        printf("Kernel error: %s\n", cudaGetErrorString(err));
    }
}
