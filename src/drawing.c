#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "../headers/boid_core.h"
#include "../headers/defs.h"
#include "../headers/drawing.h"

// shared shape templates
static const Triangle3D TRIANGLES_BASIC[] = {
    {{0, 0.3f, 0}, {-0.2f, -0.3f, -0.2f}, {0.2f, -0.3f, -0.2f}}, // front
    {{0, 0.3f, 0}, {0.2f, -0.3f, -0.2f}, {0.2f, -0.3f, 0.2f}},   // right
    {{0, 0.3f, 0}, {0.2f, -0.3f, 0.2f}, {-0.2f, -0.3f, 0.2f}},   // back
    {{0, 0.3f, 0}, {-0.2f, -0.3f, 0.2f}, {-0.2f, -0.3f, -0.2f}}, // left
};

static const BoidShape3D SHAPE_BASIC = {
    .triangles = TRIANGLES_BASIC,
    .triangle_count = 4,
};

// Reuse basic for SCOUT for now
static const BoidShape3D SHAPE_SCOUT = {
    .triangles = TRIANGLES_BASIC,
    .triangle_count = 4,
};

static const BoidShape3D *get_boid_shape(const Boid *bptr) {
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
    };
    }
}

void draw_all(Boid *boid_array) {

    Color colors[BOID_COUNT] = {0};
    ClearBackground(BLACK);

    for (int i = 0; i < BOID_COUNT; i++) {
        colors[i] = (Color){(unsigned char)GetRandomValue(0, 50),
                            (unsigned char)GetRandomValue(0, 100),
                            (unsigned char)GetRandomValue(200, 255), 255};

        Boid *bptr = &boid_array[i];

        const BoidShape3D *shape = get_boid_shape(bptr);

        if (!shape)
            continue;

        Vector3 offset = {bptr->x_pos, bptr->z_pos + 100, bptr->y_pos};
        float scale = 5.0f;

        for (int t = 0; t < shape->triangle_count; t++) {
            Vector3 v1 =
                Vector3Add(Vector3Scale(shape->triangles[t].v1, scale), offset);
            Vector3 v2 =
                Vector3Add(Vector3Scale(shape->triangles[t].v2, scale), offset);
            Vector3 v3 =
                Vector3Add(Vector3Scale(shape->triangles[t].v3, scale), offset);
            DrawTriangle3D(v1, v2, v3, colors[i]);
        }
    }
}
