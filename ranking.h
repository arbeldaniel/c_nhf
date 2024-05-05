#ifndef UNTITLED_RANKING_H
#define UNTITLED_RANKING_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>

typedef struct ranking_list {
    char name[3];
    int score;
    struct ranking_list* next;
} list;

void build_score_screen(SDL_Window** window, SDL_Renderer* renderer, int width, int height, int score);
char* enter_name(SDL_Renderer* renderer, int score, int h, int w);
void delete_string(SDL_Renderer* renderer, SDL_Rect rect);
void draw_line(SDL_Renderer* renderer, SDL_Rect rect, int d, bool delete);
void record_to_file(char* record);
list* file_to_list(FILE* fp);
list* new_list_element(char* name, int score);
list* insert_record(list* first, char* record);
void list_to_file(FILE* fp, list* first);
bool ranking_build(SDL_Renderer* renderer, int w, int h);

#endif //UNTITLED_RANKING_H
