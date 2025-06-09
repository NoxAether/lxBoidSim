#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>
#include <rcamera.h>

typedef struct {
    Camera camera;
    int camera_mode;
} Player;

Player core_camera();

void draw_player(Player player);

#endif
