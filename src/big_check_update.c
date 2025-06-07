#include <math.h>

#include "../headers/big_check_update.h"

void boid_cohesion(Boid *boid_array, int start, int end) {
    for (int i = start; i < end; i++) {
        Boid *bptri = &boid_array[i];

        bptri->xpos_avg = 0;
        bptri->ypos_avg = 0;
        bptri->xvel_avg = 0;
        bptri->yvel_avg = 0;
        bptri->neighboring_boids = 0;

        for (int j = 0; j < BOID_COUNT; j++) {
            if (j == i)
                continue;
            Boid *bptrj = &boid_array[j];

            float dx = bptri->x_pos - bptrj->x_pos;
            float dy = bptri->y_pos - bptrj->y_pos;
            float distance_squared = (dx * dx) + (dy * dy);

            if (distance_squared < VISIBLE_RANGE_SQUARED &&
                distance_squared > PROTECTED_RANGE_SQUARED) {
                bptri->xpos_avg += bptrj->x_pos;
                bptri->ypos_avg += bptrj->y_pos;
                bptri->xvel_avg += bptrj->x_vel;
                bptri->yvel_avg += bptrj->y_vel;

                bptri->neighboring_boids += 1;
            }
        }
        if (bptri->neighboring_boids > 0) {
            bptri->xpos_avg /= bptri->neighboring_boids;
            bptri->ypos_avg /= bptri->neighboring_boids;
            bptri->xvel_avg /= bptri->neighboring_boids;
            bptri->yvel_avg /= bptri->neighboring_boids;
        }

        bptri->x_vel += (bptri->xpos_avg - bptri->x_pos) * CENTERING_FACTOR;
        bptri->y_vel += (bptri->ypos_avg - bptri->y_pos) * CENTERING_FACTOR;

        bptri->x_vel += (bptri->xvel_avg - bptri->x_vel) * MATCHING_FACTOR;
        bptri->y_vel += (bptri->yvel_avg - bptri->y_vel) * MATCHING_FACTOR;
    }
}

void boid_collision(Boid *boid_array, int start, int end) {
    for (int i = start; i < end; i++) {
        Boid *bptri = &boid_array[i];
        bptri->close_dx = 0;
        bptri->close_dy = 0;

        for (int j = 0; j < BOID_COUNT; j++) {
            if (i == j) {
                continue; // skip itself
            }
            Boid *bptrj = &boid_array[j];

            float dx = bptri->x_pos - bptrj->x_pos;
            float dy = bptri->y_pos - bptrj->y_pos;
            float distance_squared = (dx * dx) + (dy * dy);

            if (distance_squared < PROTECTED_RANGE_SQUARED) {
                bptri->close_dx += dx;
                bptri->close_dy += dy;
            }
        }
        bptri->x_vel += bptri->close_dx * AVOID_FACTOR;
        bptri->y_vel += bptri->close_dy * AVOID_FACTOR;

        bptri->x_vel = bptri->x_vel;
        bptri->y_vel = bptri->y_vel;
    }

    for (int i = start; i < end; i++) {
        Boid *bptri = &boid_array[i];

        float speed =
            sqrtf(bptri->x_vel * bptri->x_vel + bptri->y_vel * bptri->y_vel);

        if (speed < MIN_SPEED) {
            bptri->x_vel = (bptri->x_vel / speed) * MIN_SPEED;
            bptri->y_vel = (bptri->y_vel / speed) * MIN_SPEED;
        }
        if (speed > MAX_SPEED) {
            bptri->x_vel = (bptri->x_vel / speed) * MAX_SPEED;
            bptri->y_vel = (bptri->y_vel / speed) * MAX_SPEED;
        }
    }
}
