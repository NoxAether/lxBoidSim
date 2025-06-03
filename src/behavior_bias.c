#include <math.h>

#include "../headers/behavior_bias.h"
#include "../headers/defs.h"

void apply_bias(Boid *boid_array) {
    for (int i = 0; i < BOID_COUNT; i++) {
        Boid *bptr = &boid_array[i];

        if (bptr->is_scout_group_1) {
            if (bptr->x_vel > 0)
                bptr->biasval = fminf(MAX_BIAS, bptr->biasval + BIAS_INCREMENT);
            else
                bptr->biasval =
                    fmaxf(BIAS_INCREMENT, bptr->biasval - BIAS_INCREMENT);

            bptr->x_vel =
                (1.0f - bptr->biasval) * bptr->x_vel + bptr->biasval * 1.0f;
        } else {
            if (bptr->x_vel < 0)
                bptr->biasval = fminf(MAX_BIAS, bptr->biasval + BIAS_INCREMENT);
            else
                bptr->biasval =
                    fmaxf(BIAS_INCREMENT, bptr->biasval - BIAS_INCREMENT);

            bptr->x_vel =
                (1.0f - bptr->biasval) * bptr->x_vel - bptr->biasval * 1.0f;
        }
    }
}
