#ifndef UNTITLED_GAME_H
#define UNTITLED_GAME_H

#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

#define pixel 30

typedef enum direction { left, right, up, down} DIRECTION;
typedef enum ghost { blinky, pinky, inky, clyde} GHOST;

typedef struct object {
    int x, y, dir;
    int r, g, b, a;
    int start_x, start_y;
} OBJECT;

typedef struct box {
    int x1, x2, y1, y2;
} BOX;

int game_main(SDL_Window** window, SDL_Renderer* renderer, int** map);
void game_build_map(SDL_Renderer* renderer, int** map, int w, int h);
int game_loop(SDL_Renderer* renderer, int w, int h, int** map, int score_to_win);
Uint32 timer(Uint32 ms, void *param);
TTF_Font* open_font(int size);
void delete_objects(SDL_Renderer* renderer, int** map);
bool move_pacman(int** map, int next_dir, int* score, int* score_to_win, bool* double_points, int w, int h);
bool move_ghost(int** map, OBJECT* object, int target_x, int target_y);
bool check_collision(OBJECT* object);
double calc_dist(int x1, int x2, int y1, int y2);
void move_object(OBJECT* object);
void draw_objects(SDL_Renderer* renderer, bool green);
void delete_score(SDL_Renderer* renderer, int w, int h);
SDL_Rect draw_string(SDL_Renderer* renderer,TTF_Font *font, char* str, int y, int w, bool middle);

#endif //UNTITLED_GAME_H
