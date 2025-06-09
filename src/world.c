#include <raylib.h>

#include "../headers/world.h"

void draw_world() {
    DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f},
              LIGHTGRAY); // Draw ground
}
