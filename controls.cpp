#include "controls.h"

// Manages player input for camera controls
void core_input(Player *player) {
    if (IsKeyDown('W'))
        CameraMoveForward(&player->camera, 1.0f, false);
    if (IsKeyDown('S'))
        CameraMoveForward(&player->camera, -1.0f, false);
    if (IsKeyDown('A'))
        CameraMoveRight(&player->camera, -1.0f, false);
    if (IsKeyDown('D'))
        CameraMoveRight(&player->camera, 1.0f, false);
}
