#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/boid_core.h"
#include "../headers/defs.h"
#include "../headers/drawing.h"
#include "../headers/thread_management.h"

int main(void) {

    srand((unsigned)time(NULL));
    printf("Debug below...\n");

    // Boid setup
    Boid *boid_array = createBoidArray();
    fprintf(stdout, "boid array setup and initialised");

    // Raylib setup
    SetTargetFPS(FPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Boids Simulation");

    generate_thread_objects(boid_array);

    while (!WindowShouldClose()) {

        // parallel boid update
        run_threads(); // Start threads
        end_threads(); // Wait for finish

        // drawing
        BeginDrawing();

        draw_all(boid_array);

        EndDrawing();
    }

    cleanup_threads();
    freeBoidArray(boid_array);
    boid_array = NULL;

    fprintf(stdout, "Memory cleaned up\n");

    CloseWindow();

    return 0;
}
