#include <math.h>

#include "../headers/boid_core.h"
#include "../headers/defs.h"
#include "../headers/small_check_update.h"

// Turning based on bounds from edge to screen?
void check_bounds(Boid *boid_array, int left_margin, int right_margin,
                  int top_margin, int bottom_margin) {
    for (int i = 0; i < BOID_COUNT; i++) {
        // Shortcut
        Boid *bptr = &boid_array[i];

        if (bptr->x_pos < left_margin)
            bptr->x_vel = bptr->x_vel + TURN_FACTOR;

        if (bptr->x_pos > right_margin)
            bptr->x_vel = bptr->x_vel - TURN_FACTOR;

        if (bptr->y_pos > bottom_margin)
            bptr->y_vel = bptr->y_vel - TURN_FACTOR;

        if (bptr->y_pos < top_margin)
            bptr->y_vel = bptr->y_vel + TURN_FACTOR;
    }
}

void limit_speed(Boid *boid_array) {
    for (int i = 0; i < BOID_COUNT; ++i) {
        Boid *bptr = &boid_array[i];

        float vx = bptr->x_vel, vy = bptr->y_vel;
        float mag2 = vx * vx + vy * vy;
        if (mag2 == 0.0f) { // dead-stop guard
            bptr->x_vel = MIN_SPEED;
            bptr->y_vel = 0.0f;
            continue;
        }
        float mag = sqrtf(mag2);
        if (mag < MIN_SPEED || mag > MAX_SPEED) {
            float k = (mag < MIN_SPEED) ? MIN_SPEED / mag : MAX_SPEED / mag;
            bptr->x_vel *= k;
            bptr->y_vel *= k;
        }
    }
}

void update_position(Boid *boid_array, float deltaTime) {
    for (int i = 0; i < BOID_COUNT; i++) {
        Boid *bptr = &boid_array[i];
        bptr->x_pos += bptr->x_vel * deltaTime * SPEED_SCALE;
        bptr->y_pos += bptr->y_vel * deltaTime * SPEED_SCALE;
    }
}
