#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Include CUDA headers first
#include <cuda_runtime.h>

// Then include other headers
#include <raylib.h>
#include <rcamera.h>

#include "boid_core.h"
#include "controls.h"
#include "defs.h"
#include "drawing.h"
#include "player.h"
#include "world.h"

// Function declarations for CUDA operations
Boid *createBoidArrayCUDA();
void freeBoidArrayCUDA(Boid *d_boids);
void runCUDAKernels(Boid *d_boids);

int main(void) {
    srand((unsigned)time(NULL));
    printf("CUDA Boids Simulation...\n");

    // Initialize CUDA device boid array
    Boid *d_boids = createBoidArrayCUDA();
    fprintf(stdout, "CUDA boid array setup and initialized\n");

    // Raylib setup
    SetTargetFPS(FPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "CUDA Boids Simulation");

    Player player = core_camera();
    DisableCursor();

    // Host copy for rendering
    Boid *h_boids = (Boid *)malloc(BOID_COUNT * sizeof(Boid));
    if (!h_boids) {
        printf("Host boid array allocation failed\n");
        return 1;
    }

    while (!WindowShouldClose()) {
        // Run CUDA kernels for simulation
        runCUDAKernels(d_boids);

        // Copy results back to host for rendering
        cudaError_t err =
            cudaMemcpy(h_boids, d_boids, BOID_COUNT * sizeof(Boid),
                       cudaMemcpyDeviceToHost);
        if (err != cudaSuccess) {
            printf("CUDA error: %s\n", cudaGetErrorString(err));
        }

        core_input(&player);
        UpdateCamera(&player.camera, player.camera_mode);

        // Drawing
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(player.camera);

        draw_world();
        draw_all(h_boids);
        draw_player(player);

        EndMode3D();
        DrawFPS(10, 10);
        EndDrawing();
    }

    // Cleanup
    freeBoidArrayCUDA(d_boids);
    free(h_boids);

    fprintf(stdout, "CUDA memory cleaned up\n");
    CloseWindow();

    return 0;
}
