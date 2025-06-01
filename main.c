#include <SDL2/SDL.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// CONSTANTS
#define TURN_FACTOR 0.2f
#define VISIBLE_RANGE 40
#define PROTECTED_RANGE 8
#define CENTERING_FACTOR 0.0005f
#define AVOID_FACTOR 0.05f
#define MATCHING_FACTOR 0.05f
#define MAX_SPEED 6.0f
#define MIN_SPEED 3.0f
#define MAX_BIAS 0.01f
#define BIAS_INCREMENT 0.00004f
#define DEFAULT_BIAS 0.001f

#define VISIBLE_RANGE_SQUARED (VISIBLE_RANGE * VISIBLE_RANGE)
#define PROTECTED_RANGE_SQUARED (PROTECTED_RANGE * PROTECTED_RANGE)
#define BOID_COUNT 2000

// Screen
#define SCREEN_WIDTH 3440
#define SCREEN_HEIGHT 1440

// The individual "bird"
typedef struct {

    // Position
    int x_pos, y_pos;
    int close_dx, close_dy;
    int xpos_avg, ypos_avg;

    // Velocity
    float x_vel, y_vel;
    float xvel_avg, yvel_avg;

    // Proximity
    int neighboring_boids;

    // Behaviour
    float biasval;
    int is_scout_group_1; // 1 or 0 for scout groups;

} Boid;

// CORE FUNCTIONS

void update_position(Boid *boid_array[]) {
    for (int i = 0; i < BOID_COUNT; i++) {
        boid_array[i]->x_pos += (int)boid_array[i]->x_vel;
        boid_array[i]->y_pos += (int)boid_array[i]->y_vel;
    }
}

// Turning based on bounds from edge to screen?
void check_bounds(Boid *boid_array[], int left_margin, int right_margin,
                  int top_margin, int bottom_margin) {
    for (int i = 0; i < BOID_COUNT; i++) {
        if (boid_array[i]->x_pos < left_margin)
            boid_array[i]->x_vel = boid_array[i]->x_vel + TURN_FACTOR;

        if (boid_array[i]->x_pos > right_margin)
            boid_array[i]->x_vel = boid_array[i]->x_vel - TURN_FACTOR;

        if (boid_array[i]->y_pos > bottom_margin)
            boid_array[i]->y_vel = boid_array[i]->y_vel - TURN_FACTOR;

        if (boid_array[i]->y_pos < top_margin)
            boid_array[i]->y_vel = boid_array[i]->y_vel + TURN_FACTOR;
    }
}

// For cohesion lol
void boid_cohesion(Boid *boid_array[]) {
    for (int i = 0; i < BOID_COUNT; i++) {
        boid_array[i]->xpos_avg = 0;
        boid_array[i]->ypos_avg = 0;
        boid_array[i]->xvel_avg = 0;
        boid_array[i]->yvel_avg = 0;
        boid_array[i]->neighboring_boids = 0;

        for (int j = 0; j < BOID_COUNT; j++) {
            int dx = boid_array[i]->x_pos - boid_array[j]->x_pos;
            int dy = boid_array[i]->y_pos - boid_array[j]->y_pos;
            int distance_squared = (dx * dx) + (dy * dy);

            if (distance_squared < VISIBLE_RANGE_SQUARED) {
                boid_array[i]->xpos_avg += boid_array[j]->x_pos;
                boid_array[i]->ypos_avg += boid_array[j]->y_pos;
                boid_array[i]->xvel_avg += boid_array[j]->x_vel;
                boid_array[i]->yvel_avg += boid_array[j]->y_vel;

                boid_array[i]->neighboring_boids += 1;
            }
        }
        if (boid_array[i]->neighboring_boids > 0) {
            boid_array[i]->xpos_avg /= boid_array[i]->neighboring_boids;
            boid_array[i]->ypos_avg /= boid_array[i]->neighboring_boids;
            boid_array[i]->xvel_avg /= boid_array[i]->neighboring_boids;
            boid_array[i]->yvel_avg /= boid_array[i]->neighboring_boids;
        }

        boid_array[i]->x_vel +=
            (boid_array[i]->xpos_avg - boid_array[i]->x_pos) * CENTERING_FACTOR;
        boid_array[i]->y_vel +=
            (boid_array[i]->ypos_avg - boid_array[i]->y_pos) * CENTERING_FACTOR;

        boid_array[i]->x_vel +=
            (boid_array[i]->xvel_avg - boid_array[i]->x_vel) * MATCHING_FACTOR;
        boid_array[i]->y_vel +=
            (boid_array[i]->yvel_avg - boid_array[i]->y_vel) * MATCHING_FACTOR;
    }
}

// Do they crash?
void boid_collision(Boid *boid_array[]) {
    for (int i = 0; i < BOID_COUNT; i++) {
        boid_array[i]->close_dx = 0;
        boid_array[i]->close_dy = 0;

        for (int j = 0; j < BOID_COUNT; j++) {
            if (i == j) {
                continue; // skip itself
            }
            int dx = boid_array[i]->x_pos - boid_array[j]->x_pos;
            int dy = boid_array[i]->y_pos - boid_array[j]->y_pos;
            int distance_squared = (dx * dx) + (dy * dy);

            if (distance_squared < PROTECTED_RANGE_SQUARED) {
                boid_array[i]->close_dx += dx;
                boid_array[i]->close_dy += dy;
            }
        }
        boid_array[i]->x_vel += boid_array[i]->close_dx * AVOID_FACTOR;
        boid_array[i]->y_vel += boid_array[i]->close_dy * AVOID_FACTOR;

        boid_array[i]->x_vel = boid_array[i]->x_vel;
        boid_array[i]->y_vel = boid_array[i]->y_vel;
    }

    for (int i = 0; i < BOID_COUNT; i++) {
        float speed = sqrtf(boid_array[i]->x_vel * boid_array[i]->x_vel +
                            boid_array[i]->y_vel * boid_array[i]->y_vel);

        if (speed < MIN_SPEED) {
            boid_array[i]->x_vel = (boid_array[i]->x_vel / speed) * MIN_SPEED;
            boid_array[i]->y_vel = (boid_array[i]->y_vel / speed) * MIN_SPEED;
        }
        if (speed > MAX_SPEED) {
            boid_array[i]->x_vel = (boid_array[i]->x_vel / speed) * MAX_SPEED;
            boid_array[i]->y_vel = (boid_array[i]->y_vel / speed) * MAX_SPEED;
        }
    }
}

// Bias logic
void apply_bias(Boid *boid_array[]) {
    for (int i = 0; i < BOID_COUNT; i++) {
        if (boid_array[i]->is_scout_group_1) {
            if (boid_array[i]->x_vel > 0)
                boid_array[i]->biasval =
                    fminf(MAX_BIAS, boid_array[i]->biasval + BIAS_INCREMENT);
            else
                boid_array[i]->biasval = fmaxf(
                    BIAS_INCREMENT, boid_array[i]->biasval - BIAS_INCREMENT);

            boid_array[i]->x_vel =
                (1.0f - boid_array[i]->biasval) * boid_array[i]->x_vel +
                boid_array[i]->biasval * 1.0f;
        } else {
            if (boid_array[i]->x_vel < 0)
                boid_array[i]->biasval =
                    fminf(MAX_BIAS, boid_array[i]->biasval + BIAS_INCREMENT);
            else
                boid_array[i]->biasval = fmaxf(
                    BIAS_INCREMENT, boid_array[i]->biasval - BIAS_INCREMENT);

            boid_array[i]->x_vel =
                (1.0f - boid_array[i]->biasval) * boid_array[i]->x_vel -
                boid_array[i]->biasval * 1.0f;
        }
    }
}

// Creating the boids to be used in the simulation
void create_boid_array(Boid *boid_array[]) {
    for (int i = 0; i < BOID_COUNT; i++) {
        boid_array[i] = malloc(sizeof(Boid));
        if (boid_array[i] == NULL) {
            printf("Boid array creation unsuccessful\n");
            exit(0);
        }
        // Initialise values inside the struct
        boid_array[i]->x_pos = rand() % 100;
        boid_array[i]->y_pos = rand() % 100;
        boid_array[i]->x_vel = 0.0f;
        boid_array[i]->y_vel = 0.0f;
        boid_array[i]->close_dx = 0;
        boid_array[i]->close_dy = 0;
        boid_array[i]->xpos_avg = 0;
        boid_array[i]->ypos_avg = 0;
        boid_array[i]->neighboring_boids = 0;
        boid_array[i]->biasval = DEFAULT_BIAS;
        boid_array[i]->is_scout_group_1 =
            rand() % 2; // Randomly assign to scout group 1 or 2
    }
    printf("Boid array creation successful\n");
}

void free_all(Boid *boid_array[]) {
    for (int i = 0; i < BOID_COUNT; i++) {
        free(boid_array[i]);
    }
    printf("All Boid structs successfully freed\n");
}

int main(void) {
    printf("Debug below...\n");

    // Boid setup
    Boid *boid_array[BOID_COUNT];
    create_boid_array(boid_array);

    // Margins for boundary checking
    int left_margin = 50;
    int right_margin = SCREEN_WIDTH - 50;
    int top_margin = 50;
    int bottom_margin = SCREEN_HEIGHT - 50;

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n",
                SDL_GetError());
        free_all(boid_array);
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Boids Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_Quit();
        free_all(boid_array);
        return 1;
    }

    SDL_Renderer *renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n",
                SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        free_all(boid_array);
        return 1;
    }

    int running = 1;
    SDL_Event event;

    printf("Program Running\n");
    while (running) {
        // Event handling
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = 0;
        }

        // Boid logic
        boid_cohesion(boid_array);
        boid_collision(boid_array);
        apply_bias(boid_array);
        check_bounds(boid_array, left_margin, right_margin, top_margin,
                     bottom_margin);
        update_position(boid_array);

        // Rendering
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White boids
        for (int i = 0; i < BOID_COUNT; i++) {
            SDL_Rect boid_rect = {
                boid_array[i]->x_pos, boid_array[i]->y_pos, 4,
                4 // Size of the boid
            };
            SDL_RenderFillRect(renderer, &boid_rect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }

    printf("Program Ending\n");
    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    free_all(boid_array);
    return 0;
}
