#ifndef BOID_CORE_H
#define BOID_CORE_H

// The individual "bird"
typedef struct {

    // Position
    float x_pos, y_pos;
    float close_dx, close_dy;
    float xpos_avg, ypos_avg;

    // Velocity
    float x_vel, y_vel;
    float xvel_avg, yvel_avg;

    // Proximity
    int neighboring_boids;

    // Behaviour
    float biasval;
    int is_scout_group_1; // 1 or 0 for scout groups;

} Boid;

Boid *createBoidArray(void);
void freeBoidArray(Boid *flock);

#endif
