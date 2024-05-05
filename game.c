#include "game.h"

OBJECT ghost[4] = { {0, 0, 5, 255, 0, 0, 255, 0, 0},
                    {0, 0, 5, 255, 184, 255, 255, 0, 0},
                    {0, 0, 5, 0, 255, 255, 255, 0, 0},
                    {0, 0, 5, 255, 184, 82, 255, 0, 0}};
OBJECT pacman = {0, 0, 5, 255, 238, 0, 255, 0, 0};

int game_main(SDL_Window** window, SDL_Renderer* renderer, int** map) {
    int height = 0, width = 0;
    for(height = 0; map[height] != NULL; ++height);
    height = (height / 2) + 1;
    for(width = 0; map[0][width] <= 'x'; ++width);
    printf("%d, %d\n", height, width);

    SDL_SetWindowSize(*window, width * pixel, height * pixel);
    game_build_map(renderer, map, width, height - 1);

    int score_to_win = 0;
    for(int y = 0; y < height - 1; ++y) {
        for(int x = 0; x < width; ++x) {
            if(map[y][x] == 2 || map[y][x] == 3)
                ++score_to_win;
        }
    }

    int score = game_loop(renderer, width, height, map, score_to_win);

    return score;
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
void game_build_map(SDL_Renderer* renderer, int** map, int w, int h) {
    BOX tmpbox;
    for(int y = 0; y < h; ++y) {
        for(int x = 0; x < w; ++x) {
            switch (map[y][x] + '0') {
                case 'b': ghost[blinky].x = x;
                    ghost[blinky].start_x = x;
                    ghost[blinky].y = y;
                    ghost[blinky].start_y = y;
                    map[y][x] = 0;
                    break;
                case 'p': ghost[pinky].x = x;
                    ghost[pinky].start_x = x;
                    ghost[pinky].y = y;
                    ghost[pinky].start_y = y;
                    map[y][x] = 0;
                    break;
                case 'i': ghost[inky].x = x;
                    ghost[inky].start_x = x;
                    ghost[inky].y = y;
                    ghost[inky].start_y = y;
                    map[y][x] = 0;
                    break;
                case 'c': ghost[clyde].x = x;
                    ghost[clyde].start_x = x;
                    ghost[clyde].y = y;
                    ghost[clyde].start_y = y;
                    map[y][x] = 0;
                    break;
                case 'x': pacman.x = x;
                    pacman.y = y;
                    map[y][x] = 0;
                    break;
                default:
                    tmpbox.x1 = x * pixel;
                    tmpbox.x2 = (x+1) * pixel -1;
                    tmpbox.y1 = y * pixel;
                    tmpbox.y2 = (y+1) * pixel -1;
                    switch (map[y][x]) {
                        case 1: boxRGBA(renderer, tmpbox.x1, tmpbox.y1, tmpbox.x2, tmpbox.y2, 50, 50, 253, 255);
                            break;
                        case 2: filledCircleRGBA(renderer, tmpbox.x1 + (pixel / 2), tmpbox.y1 + (pixel / 2), pixel / 10, 253, 253, 150, 255);
                            break;
                        case 3: filledCircleRGBA(renderer, tmpbox.x1 + (pixel / 2), tmpbox.y1 + (pixel / 2), pixel / 4, 253, 253, 150, 255);
                            break;
                        default: break;
                    }
                    break;
            }
        }
    }
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
int game_loop(SDL_Renderer* renderer, int w, int h, int** map, int score_to_win) {
    bool quit = false, skip = false;
    int next_dir = 5;
    int score = 0;
    int score_start = score_to_win;
    bool pac_mode = false, pac_mode_wait = false;
    bool enable_inky = false, enable_clyde = false;
    int StartTicks, EndTicks;

    //draw_objects(renderer);
    SDL_TimerID id = SDL_AddTimer(350, timer, NULL);
    TTF_Font* font = open_font(1.5 * pixel);

    while(!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        if(score_to_win <= ((2 * score_start) / 3))
            enable_inky = true;
        if(score_to_win <= (score_start / 3))
            enable_clyde = true;

        switch(event.type) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT: next_dir = left;
                        skip = true;
                        break;
                    case SDLK_RIGHT: next_dir = right;
                        skip = true;
                        break;
                    case SDLK_UP: next_dir = up;
                        skip = true;
                        break;
                    case SDLK_DOWN: next_dir = down;
                        skip = true;
                        break;
                    case SDLK_RETURN:
                        skip = true;
                        quit = true;
                        break;
                    default: break;
                }
                break;
            case SDL_KEYUP:
                skip = true;
                break;
            case SDL_QUIT:
                skip = true;
                quit = true;
                break;
            default: break;
        }

        if(skip) {
            skip = false;
            continue;
        }

        delete_objects(renderer, map);

        if(!quit)
            quit = move_pacman(map, next_dir, &score, &score_to_win, &pac_mode, w, h);
        if(pac_mode_wait) {
            EndTicks = SDL_GetTicks();
            if((EndTicks - StartTicks) >= 5000)
                pac_mode_wait = false;
        }
        if(pac_mode) {
            StartTicks = SDL_GetTicks();
            pac_mode = false;
            pac_mode_wait = true;
        }
        if(!quit)
            quit = move_ghost(map, &ghost[blinky], pacman.x, pacman.y);
        if(!quit && enable_clyde) {
            if(calc_dist(ghost[clyde].x, pacman.x, ghost[clyde].y, pacman.y) < 8)
                quit = move_ghost(map, &ghost[clyde], 0, h);
            else
                quit = move_ghost(map, &ghost[clyde], pacman.x, pacman.y);
        }
        switch(pacman.dir) {
            case left:
                if(!quit)
                    quit = move_ghost(map, &ghost[pinky], pacman.x - 3, pacman.y);
                if(!quit && enable_inky)
                    quit = move_ghost(map, &ghost[inky], pacman.x - 2 - (fabs(pacman.x - 2 - ghost[blinky].x)), pacman.y);
                break;
            case right:
                if(!quit)
                    quit = move_ghost(map, &ghost[pinky], pacman.x + 3, pacman.y);
                if(!quit && enable_inky)
                    quit = move_ghost(map, &ghost[inky], pacman.x + 2 + (fabs(pacman.x + 2 - ghost[blinky].x)), pacman.y);
                break;
            case up:
                if(!quit)
                    quit = move_ghost(map, &ghost[pinky], pacman.x - 3, pacman.y - 3);
                if(!quit && enable_inky)
                    quit = move_ghost(map, &ghost[inky], pacman.x, pacman.y - 2 - (fabs(pacman.y - 2 - ghost[blinky].y)));
                break;
            case down:
                if(!quit)
                    quit = move_ghost(map, &ghost[pinky], pacman.x, pacman.y + 3);
                if(!quit && enable_inky)
                    quit = move_ghost(map, &ghost[inky], pacman.x, pacman.y + 2 + (fabs(pacman.y + 2 - ghost[blinky].y)));
                break;
            default: break;
        }
        if(quit && pac_mode_wait) {
            for(int i = 0; i < 4; ++i) {
                if(ghost[i].x == pacman.x && ghost[i].y == pacman.y) {
                    ghost[i].x = ghost[i].start_x;
                    ghost[i].y = ghost[i].start_y;
                    score += 800;
                    quit = false;
                }
            }
        }

        draw_objects(renderer, pac_mode_wait);
        delete_score(renderer, w, h);
        char str[18] = "score", tmp[12];
        sprintf(tmp, "   %d", score);
        strcat(str, tmp);
        draw_string(renderer, font, str, (h-1) * pixel - 10, w, false);
        SDL_RenderPresent(renderer);
    }
    SDL_RemoveTimer(id);
    TTF_CloseFont(font);
    return score;
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
Uint32 timer(Uint32 ms, void *param) {
    SDL_Event ev;
    ev.type = SDL_KEYDOWN;
    SDL_PushEvent(&ev);
    return ms;
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
TTF_Font* open_font(int size) {
    TTF_Init();
    TTF_Font *font = TTF_OpenFont("ARCADECLASSIC.TTF", size);
    if (!font) {
        SDL_Log("Can't open font.\nexiting program");
        exit(1);
    }
    return font;
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
void delete_objects(SDL_Renderer* renderer, int** map) {
    boxRGBA(renderer, pacman.x * pixel, pacman.y * pixel, (pacman.x + 1) * pixel - 1, (pacman.y + 1) * pixel -1, 0, 0, 0, 255);
    for(int i = 0; i < 4; ++i) {
        boxRGBA(renderer, ghost[i].x * pixel, ghost[i].y * pixel, (ghost[i].x + 1) * pixel -1, (ghost[i].y + 1) * pixel - 1, 0, 0, 0, 255);
        if(map[ghost[i].y][ghost[i].x] == 2)
            filledCircleRGBA(renderer, ghost[i].x * pixel + (pixel / 2), ghost[i].y * pixel + (pixel / 2), pixel / 10, 253, 253, 150, 255);
        if(map[ghost[i].y][ghost[i].x] == 3)
            filledCircleRGBA(renderer, ghost[i].x * pixel + (pixel / 2), ghost[i].y * pixel + (pixel / 2), pixel / 4, 253, 253, 150, 255);
    }
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
bool move_pacman(int** map, int next_dir, int* score, int* score_to_win, bool* pac_mode, int w, int h) {
    if(pacman.dir != next_dir) {
        switch (next_dir) {
            case left:
                if(map[pacman.y][pacman.x-1] != 1)
                    pacman.dir = next_dir;
                break;
            case right:
                if(map[pacman.y][pacman.x+1] != 1)
                    pacman.dir = next_dir;
                break;
            case up:
                if(map[pacman.y-1][pacman.x] != 1)
                    pacman.dir = next_dir;
                break;
            case down:
                if(map[pacman.y+1][pacman.x] != 1)
                    pacman.dir = next_dir;
                break;
            default: break;
        }
    }

    switch(pacman.dir) {
        case left:
            if(map[pacman.y][pacman.x-1] == 1)
                break;
            if(map[pacman.y][pacman.x] == 2) {
                --*score_to_win;
                *score += 10;
            }
            if(map[pacman.y][pacman.x] == 3) {
                *pac_mode = true;
                --*score_to_win;
            }
            if(map[pacman.y][pacman.x] == 4) {
                if(pacman.x == 0)
                    pacman.x = w-1;
                else
                    pacman.x -= 1;
                break;
            }
            map[pacman.y][pacman.x] = 0;
            pacman.x -= 1;
            break;

        case right:
            if(map[pacman.y][pacman.x+1] == 1)
                break;
            if(map[pacman.y][pacman.x] == 2) {
                --*score_to_win;
                *score += 10;
            }
            if(map[pacman.y][pacman.x] == 3) {
                *pac_mode = true;
                --*score_to_win;
            }
            if(map[pacman.y][pacman.x] == 4) {
                if(pacman.x == w-1)
                    pacman.x = 0;
                else
                    pacman.x += 1;
                break;
            }
            map[pacman.y][pacman.x] = 0;
            pacman.x += 1;
            break;

        case up:
            if(map[pacman.y-1][pacman.x] == 1)
                break;
            if(map[pacman.y][pacman.x] == 2) {
                --*score_to_win;
                *score += 10;
            }
            if(map[pacman.y][pacman.x] == 3) {
                *pac_mode = true;
                --*score_to_win;
            }
            if(map[pacman.y][pacman.x] == 4) {
                if(pacman.y == 0)
                    pacman.y = h-1;
                else
                    pacman.y -= 1;
                break;
            }
            map[pacman.y][pacman.x] = 0;
            pacman.y -= 1;
            break;

        case down:
            if(map[pacman.y+1][pacman.x] == 1)
                break;
            if(map[pacman.y][pacman.x] == 2) {
                --*score_to_win;
                *score += 10;
            }
            if(map[pacman.y][pacman.x] == 3) {
                *pac_mode = true;
                --*score_to_win;
            }
            if(map[pacman.y][pacman.x] == 4) {
                if(pacman.y == h-1)
                    pacman.y = 0;
                else
                    pacman.y += 1;
                break;
            }
            map[pacman.y][pacman.x] = 0;
            pacman.y += 1;
            break;
    }
    if(*score_to_win <= 0)
        return true;
    return false;
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
bool move_ghost(int** map, OBJECT* object, int target_x, int target_y) {
    int dir[4] = {1, 1, 1, 1};
    double distance[4] = {100, 100, 100, 100};
    bool end = false;

    if (check_collision(object) == true)
        return true;

    switch(object->dir) {
        case left:
            dir[right] = 0;
            if(map[object->y][object->x - 1] == 4) {
                object->dir = right;
                end = true;
            }
            break;
        case right:
            dir[left] = 0;
            if(map[object->y][object->x + 1] == 4) {
                object->dir = left;
                end = true;
            }
            break;
        case up:
            dir[down] = 0;
            if(map[object->y - 1][object->x] == 4) {
                object->dir = down;
                end = true;
            }
            break;
        case down:
            dir[up] = 0;
            if(map[object->y + 1][object->x] == 4) {
                object->dir = up;
                end = true;
            }
            break;
    }

    if(end) {
        move_object(object);
        return check_collision(object);
    }

    if(map[object->y][object->x - 1] == 1)
        dir[left] = 0;
    if(map[object->y][object->x + 1] == 1)
        dir[right] = 0;
    if(map[object->y - 1][object->x] == 1)
        dir[up] = 0;
    if(map[object->y + 1][object->x] == 1)
        dir[down] = 0;


    if(dir[left] == 1)
        distance[left] = calc_dist(object->x - 1, target_x, object->y, target_y);
    if(dir[right] == 1)
        distance[right] = calc_dist(object->x + 1, target_x, object->y, target_y);
    if(dir[up] == 1)
        distance[up] = calc_dist(object->x, target_x, object->y - 1, target_y);
    if(dir[down] == 1)
        distance[down] = calc_dist(object->x, target_x, object->y + 1, target_y);

    if(distance[up] <= distance[left] && distance[up] <= distance[right] && distance[up] <= distance[down])
        object->dir = up;
    else {
        if (distance[left] <= distance[right] && distance[left] <= distance[down])
            object->dir = left;
        else {
            if (distance[down] <= distance[right])
                object->dir = down;
            else
                object->dir = right;
        }
    }

    move_object(object);
    return check_collision(object);
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
bool check_collision(OBJECT* object) {
    if(pacman.x == object->x && pacman.y == object->y)
        return true;
    return false;
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
double calc_dist(int x1, int x2, int y1, int y2) {
    int x, y;
    x = fabs(x1 - x2);
    y = fabs(y1 - y2);
    return sqrt(x*x + y*y);
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
void move_object(OBJECT* object) {
    switch (object->dir) {
        case left: object->x -= 1;
            break;
        case right: object->x += 1;
            break;
        case up: object->y -= 1;
            break;
        case down: object->y +=1;
            break;
        default: break;
    }
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
void draw_objects(SDL_Renderer* renderer, bool green) {
    if(green)
        boxRGBA(renderer, pacman.x * pixel, pacman.y * pixel, (pacman.x + 1) * pixel - 1, (pacman.y + 1) * pixel -1, 0, 255, 0, 255);
    else
        boxRGBA(renderer, pacman.x * pixel, pacman.y * pixel, (pacman.x + 1) * pixel - 1, (pacman.y + 1) * pixel -1, pacman.r, pacman.g, pacman.b, pacman.a);
    for(int i = 0; i < 4; ++i) {
        boxRGBA(renderer, ghost[i].x * pixel, ghost[i].y * pixel, (ghost[i].x + 1) * pixel -1, (ghost[i].y + 1) * pixel - 1, ghost[i].r, ghost[i].g, ghost[i].b, ghost[i].a);
    }
    //SDL_RenderPresent(renderer);
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
void delete_score(SDL_Renderer* renderer, int w, int h) {
    boxRGBA(renderer, 0, (h - 1) * pixel, w * pixel, h * pixel, 0, 0, 0, 255);
}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
SDL_Rect draw_string(SDL_Renderer* renderer,TTF_Font *font, char* str, int y, int w, bool middle) {
    SDL_Color white = {255, 255, 255};
    SDL_Surface* sign_surface;
    SDL_Texture* sign_texture;
    SDL_Rect sign_rect = {0, y, 0, 0};
    sign_surface = TTF_RenderUTF8_Solid(font, str, white);
    sign_texture = SDL_CreateTextureFromSurface(renderer, sign_surface);
    if(middle)
        sign_rect.x = (w - sign_surface->w) / 2;
    else
        sign_rect.x = 10;
    sign_rect.w = sign_surface->w;
    sign_rect.h = sign_surface->h;
    SDL_RenderCopy(renderer, sign_texture, NULL, &sign_rect);
    SDL_FreeSurface(sign_surface);
    SDL_DestroyTexture(sign_texture);

    return sign_rect;
}