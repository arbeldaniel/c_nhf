#include "menu.h"

SIGN signs[] = {
        {"PLAY", 0, 0, 0},
        {"RANKING", 0, 0, 0},
        {"EXIT", 0, 0, 0}
};

void menu_build(SDL_Renderer* renderer, int w, int h)  {
    SDL_Texture *menu_pic = IMG_LoadTexture(renderer, "menu.png");
    if(menu_pic == NULL) {
        SDL_Log("Can't open menu background. \nexiting program");
        exit(1);
    }
    SDL_Rect dest = {0, 0, w, h};
    SDL_RenderCopy(renderer, menu_pic, NULL, &dest);
    SDL_DestroyTexture(menu_pic);

    SDL_Color white = {255, 255, 255};

    TTF_Init();
    TTF_Font *font = TTF_OpenFont("ARCADECLASSIC.TTF", 70);
    if (!font) {
        SDL_Log("Can't open font.\nexiting program");
        exit(1);
    }

    SDL_Surface* sign_surface;
    SDL_Texture* sign_texture;
    SDL_Rect sign_rect = {0, 390, 0, 0};

    for(int i = 0; i < 3; ++i) {
        sign_surface = TTF_RenderUTF8_Solid(font, signs[i].str, white);
        sign_texture = SDL_CreateTextureFromSurface(renderer, sign_surface);
        sign_rect.x = (w - sign_surface->w) / 2;
        sign_rect.w = sign_surface->w;
        sign_rect.h = sign_surface->h;
        SDL_RenderCopy(renderer, sign_texture, NULL, &sign_rect);
        sign_rect.y += 70;

        signs[i].x = sign_rect.x;
        signs[i].y = sign_rect.y - 35;
        signs[i].l = sign_surface->w;
    }
    SDL_FreeSurface(sign_surface);
    SDL_DestroyTexture(sign_texture);

    SDL_RenderPresent(renderer);

    TTF_CloseFont(font);
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

int menu_run(SDL_Renderer* renderer) {
    bool quit = false;
    int state = PLAY;
    SDL_Color black  = {0, 0, 0},
              yellow = {255, 238, 0};

    menu_choose(renderer, signs[state], yellow);

    while(!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        if(event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_DOWN:
                    menu_choose(renderer, signs[state], black);
                    state++;
                    if(state == 3) state = PLAY;
                    menu_choose(renderer, signs[state], yellow);
                    break;
                case SDLK_UP:
                    menu_choose(renderer, signs[state], black);
                    state--;
                    if(state == -1) state = EXIT;
                    menu_choose(renderer, signs[state], yellow);
                    break;
                case SDLK_RETURN:
                    quit = true;
                    break;
                default: break;
            }
        }
        if(event.type == SDL_QUIT)
            return 'x';
    }
    return state;
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

void menu_choose(SDL_Renderer* renderer, SIGN sign, SDL_Color color) {
    int x_end = sign.x + sign.l;
    boxRGBA(renderer, sign.x, sign.y + 25, x_end, sign.y + 30, color.r, color.g, color.b, 255);
    SDL_RenderPresent(renderer);
}