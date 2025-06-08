#ifndef DRAWING_H
#define DRAWING_H

#include "../headers/boid_core.h"

#include <raylib.h>

// The object to be drawn
typedef struct {
    Vector2 v1, v2, v3;

} Triangle;

void draw_all(Boid *boid_array);

#endif
