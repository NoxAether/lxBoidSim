#include "player.h"

// Sets up the initial 3D camera
Player core_camera() {
    Player p;
    p.camera.position = (Vector3){0.0f, 150.0f, 250.0f};
    p.camera.target = (Vector3){0.0f, 150.0f, 0.0f};
    p.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    p.camera.fovy = 60.0f;
    p.camera.projection = CAMERA_PERSPECTIVE;
    p.camera_mode = CAMERA_FREE;
    return p;
}

// Placeholder for drawing the player model (optional)
void draw_player(Player player) {
    // You could draw a model or a shape for the player here if you wanted.
    // For now, it can be empty.
}
