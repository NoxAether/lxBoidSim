#ifndef SMALL_CHECK_UPDATE_H
#define SMALL_CHECK_UPDATE_H

#include "boid_core.h"

void check_bounds(Boid *boid_array, int left_margin, int right_margin,
                  int top_margin, int bottom_margin, int start, int end);

void limit_speed(Boid *boid_array, int start, int end);

void update_position(Boid *boid_array, int start, int end);

#endif
