#include <math.h>
#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/boid_core.h"
#include "../headers/defs.h"

Boid *createBoidArray(void) {
    Boid *boid_array = malloc(BOID_COUNT * sizeof *boid_array);
    if (!boid_array) {
        printf("Boid array creation unsuccessful\n");
        exit(0);
    }
    float angle = ((float)rand() / (float)RAND_MAX) * 2.0f * PI;
    float speed =
        MIN_SPEED + ((float)rand() / (float)RAND_MAX) * (MAX_SPEED - MIN_SPEED);

    for (int i = 0; i < BOID_COUNT; i++) {
        Boid *bptr = &boid_array[i];

        // Initialise values inside the struct
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
    }

    printf("Boid array creation successful\n");
    return boid_array;
}

void freeBoidArray(Boid *boid_array) {
    free(boid_array);
    fprintf(stdout, "Boid array freed");
}
