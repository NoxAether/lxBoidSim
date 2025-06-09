#include <raylib.h>
#include <rcamera.h>

#include "../headers/player.h"

Player core_camera() {
    Player player = {0};

    player.camera.position = (Vector3){0.0f, 2.0f, 4.0f};
    player.camera.target = (Vector3){0.0f, 2.0f, 0.0f};
    player.camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    player.camera.fovy = 60.0f;
    player.camera.projection = CAMERA_PERSPECTIVE;

    player.camera_mode = CAMERA_FIRST_PERSON;

    return player;
}

void draw_player(Player player) {
    if (player.camera_mode == CAMERA_THIRD_PERSON) {
        DrawCube(player.camera.target, 0.5f, 0.5f, 0.5f, PURPLE);
        DrawCubeWires(player.camera.target, 0.5f, 0.5f, 0.5f, DARKPURPLE);
    }
}
