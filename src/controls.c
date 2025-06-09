#include "../headers/controls.h"
#include "../headers/player.h"

void core_input(Player *player) {

    if (IsKeyPressed(KEY_ONE)) {
        player->camera_mode = CAMERA_FREE;
        player->camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    if (IsKeyPressed(KEY_TWO)) {
        player->camera_mode = CAMERA_FIRST_PERSON;
        player->camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    if (IsKeyPressed(KEY_THREE)) {
        player->camera_mode = CAMERA_THIRD_PERSON;
        player->camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    if (IsKeyPressed(KEY_FOUR)) {
        player->camera_mode = CAMERA_ORBITAL;
        player->camera.up = (Vector3){0.0f, 1.0f, 0.0f}; // Reset roll
    }

    // Switch player->camera projection
    if (IsKeyPressed(KEY_P)) {
        if (player->camera.projection == CAMERA_PERSPECTIVE) {
            // Create isometric view
            player->camera_mode = CAMERA_THIRD_PERSON;
            // Note: The target distance is related to the render distance in
            // the orthographic projection
            player->camera.position = (Vector3){0.0f, 2.0f, -100.0f};
            player->camera.target = (Vector3){0.0f, 2.0f, 0.0f};
            player->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
            player->camera.projection = CAMERA_ORTHOGRAPHIC;
            player->camera.fovy =
                20.0f; // near plane width in CAMERA_ORTHOGRAPHIC
            CameraYaw(&player->camera, -135 * DEG2RAD, true);
            CameraPitch(&player->camera, -45 * DEG2RAD, true, true, false);
        } else if (player->camera.projection == CAMERA_ORTHOGRAPHIC) {
            // Reset to default view
            player->camera_mode = CAMERA_THIRD_PERSON;
            player->camera.position = (Vector3){0.0f, 2.0f, 10.0f};
            player->camera.target = (Vector3){0.0f, 2.0f, 0.0f};
            player->camera.up = (Vector3){0.0f, 1.0f, 0.0f};
            player->camera.projection = CAMERA_PERSPECTIVE;
            player->camera.fovy = 60.0f;
        }
    }
}
