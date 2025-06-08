#include <raylib.h>
#include <stdio.h>

#include "../headers/boid_core.h"
#include "../headers/defs.h"
#include "../headers/drawing.h"

// shared shape templates
static const Triangle SHAPE_BASIC = {
    .v1 = {0.0f, -0.5f},
    .v2 = {-0.5f, 0.5f},
    .v3 = {0.5f, 0.5f},
};

static const Triangle SHAPE_SCOUT = {
    .v1 = {0.0f, -0.7f}, .v2 = {-0.6f, 0.6f}, .v3 = {0.6f, 0.6f}};

static const Triangle *get_boid_shape(const Boid *bptr) {
    // Get the boid type
    switch (bptr->type) {

    case BOID_TYPE_SCOUT: {
        return &SHAPE_SCOUT;
    };
    case BOID_TYPE_BASIC: {
        return &SHAPE_BASIC;
    };
    default: {
        fprintf(stderr, "No shape could be determined\n");
        return NULL;
    }
    }
}

void draw_all(Boid *boid_array) {

    ClearBackground(BLACK);

    for (int i = 0; i < BOID_COUNT; i++) {
        Boid *bptr = &boid_array[i];
        const Triangle *shape = get_boid_shape(bptr);

        Vector2 top = {bptr->x_pos + shape->v1.x,
                       bptr->y_pos + shape->v1.y}; // top

        Vector2 bot_left = {bptr->x_pos + shape->v2.x,
                            bptr->y_pos + shape->v2.y}; // bottom left

        Vector2 bot_right = {bptr->x_pos + shape->v3.x,
                             bptr->y_pos + shape->v3.y}; // bottom right

        DrawTriangleLines(top, bot_left, bot_right, WHITE);
    }
}
