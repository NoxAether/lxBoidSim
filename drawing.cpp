#include "drawing.h"
#include "defs.h"
#include <raylib.h>

// Use instanced rendering for better performance
void draw_all(Boid *boid_array) {
    if (!boid_array)
        return;

    // Use points for better performance with many boids
    for (int i = 0; i < BOID_COUNT; i++) {
        Boid *b = &boid_array[i];
        Vector3 boidPos = {(float)b->x_pos, (float)b->y_pos, (float)b->z_pos};

        // Use simple points instead of spheres for better performance
        DrawPoint3D(boidPos, MAROON);

        // Optional: Draw direction indicator
        Vector3 direction = {boidPos.x + b->x_vel * 0.5f,
                             boidPos.y + b->y_vel * 0.5f, boidPos.z};
        DrawLine3D(boidPos, direction, RED);
    }
}
