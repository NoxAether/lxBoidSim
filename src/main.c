#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../headers/behavior_bias.h"
#include "../headers/big_check_update.h"
#include "../headers/boid_core.h"
#include "../headers/defs.h"
#include "../headers/small_check_update.h"

int main(void) {

    srand((unsigned)time(NULL));
    printf("Debug below...\n");

    // Boid setup
    Boid *boid_array = createBoidArray();
    fprintf(stdout, "boid array setup and initialised");

    // Raylib setup
    SetTargetFPS(FPS);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Boids Simulation");

    // Margins for boundary checking
    int left_margin = 50;
    int right_margin = SCREEN_WIDTH - 50;
    int top_margin = 50;
    int bottom_margin = SCREEN_HEIGHT - 50;

    while (!WindowShouldClose()) {
        float deltaTime = 1.0f / (float)FPS;

        boid_collision(boid_array);
        boid_cohesion(boid_array);
        apply_bias(boid_array);
        limit_speed(boid_array);
        update_position(boid_array, deltaTime);
        check_bounds(boid_array, left_margin, right_margin, top_margin,
                     bottom_margin);

        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < BOID_COUNT; i++) {
            Vector2 v1 = {boid_array[i].x_pos,
                          boid_array[i].y_pos - 4.0f}; // top
            Vector2 v2 = {boid_array[i].x_pos - 4.0f,
                          boid_array[i].y_pos + 4.0f}; // bottom left
            Vector2 v3 = {boid_array[i].x_pos + 4.0f,
                          boid_array[i].y_pos + 4.0f}; // bottom right

            DrawTriangleLines(v1, v2, v3, WHITE);
        }

        EndDrawing();
    }

    freeBoidArray(boid_array);
    boid_array = NULL;

    fprintf(stdout, "Memory cleaned up\n");

    CloseWindow();

    return 0;
}
