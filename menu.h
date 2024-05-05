#ifndef UNTITLED_MENU_H
#define UNTITLED_MENU_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

typedef enum state {PLAY, RANKING, EXIT} STATE;

typedef struct sign {
    const char* str;
    int x, y, l;
} SIGN;

void menu_build(SDL_Renderer* renderer,int w, int h);
int menu_run(SDL_Renderer* renderer);
void menu_choose(SDL_Renderer* renderer, SIGN sign, SDL_Color color);

#endif //UNTITLED_MENU_H
