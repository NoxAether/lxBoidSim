#ifndef SMALL_CHECK_UPDATE_H
#define CHECK_POS_H

#include "boid_core.h"

void check_bounds(Boid *boid_array, int left_margin, int right_margin,
                  int top_margin, int bottom_margin);

void limit_speed(Boid *boid_array);

void update_position(Boid *boid_array, float deltaTime);

#endif
