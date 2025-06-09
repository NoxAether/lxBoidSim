#ifndef BOID_CORE_H
#define BOID_CORE_H

// The individual boid

// The different types of boid
typedef enum { BOID_TYPE_BASIC, BOID_TYPE_SCOUT } TYPE;

typedef struct {

    // Position
    float x_pos, y_pos, z_pos;
    float close_dx, close_dy;
    float xpos_avg, ypos_avg;

    // Velocity
    float x_vel, y_vel, z_vel;
    float xvel_avg, yvel_avg;

    // Proximity
    int neighboring_boids;

    // Behaviour
    float biasval;
    int is_scout_group_1; // 1 or 0 for scout groups;

    // boid type
    TYPE type;

} Boid;

Boid *createBoidArray(void);
void freeBoidArray(Boid *flock);

#endif
