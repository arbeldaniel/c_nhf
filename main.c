#include "menu.h"
#include "game.h"
#include "file.h"
#include "ranking.h"

void sdl_init(char const* title, int width, int height, SDL_Window** pwindow, SDL_Renderer** prenderer);
void blackout(SDL_Renderer* renderer, int w, int h);
void free_map(int** map);

int main() {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int option;
    int width = 560, height = 620;
    bool quit = false;
    int** map;
    int score;
    char* record;

    sdl_init("HERR-PAC", width, height, &window, &renderer);

    while(!quit) {
        blackout(renderer, width, height);
        menu_build(renderer, width, height);
        option = menu_run(renderer);
        switch (option) {
            case PLAY:
                blackout(renderer, width, height);
                map = read_map();
                score = game_main(&window, renderer, map);
                build_score_screen(&window, renderer, width, height, score);
                record = enter_name(renderer, score, height, width);
                record_to_file(record);
                free_map(map);
                break;
            case RANKING:
                blackout(renderer, width, height);
                quit = ranking_build(renderer, width, height);
                break;
            case EXIT:
                SDL_Quit();
                return 0;
            case 'x':
                quit = true;
                break;
            default:
                break;
        }
    }
    SDL_Quit();
    return 0;
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

void sdl_init(char const* title, int width, int height, SDL_Window** pwindow, SDL_Renderer** prenderer) {
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        SDL_Log("Can't start SDL: %s", SDL_GetError());
        exit(1);
    }
    SDL_Window* window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    if (window == NULL) {
        SDL_Log("Can't create window: %s", SDL_GetError());
        exit(1);
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    if (renderer == NULL) {
        SDL_Log("Can't create renderer: %s", SDL_GetError());
        exit(1);
    }
    SDL_RenderClear(renderer);

    *pwindow = window;
    *prenderer = renderer;
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

void blackout(SDL_Renderer* renderer, int width, int height) {
    boxRGBA(renderer, 0, 0, width, height, 0, 0, 0, 255);
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

void free_map(int** map) {
    int j = 0;
    for(j = 0; map[j] != NULL; ++j);
    j = j / 2;
    for(int i = 0; i < j; ++i)
        free(map[i]);
    free(map);
}