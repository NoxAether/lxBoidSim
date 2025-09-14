#ifndef DRAWING_H
#define DRAWING_H

#include "boid_core.h"

#include <raylib.h>

// The object to be drawn
typedef struct {
    Vector3 v1, v2, v3;

} Triangle3D;

typedef struct {
    const Triangle3D *triangles;
    int triangle_count;
} BoidShape3D;

void draw_all(Boid *boid_array);

#endif
