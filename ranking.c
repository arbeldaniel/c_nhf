#include "ranking.h"
#include "game.h"


void build_score_screen(SDL_Window **window, SDL_Renderer* renderer, int width, int height, int score) {
    SDL_SetWindowSize(*window, width, height);
    boxRGBA(renderer, 0, 0, width, height, 0, 0, 0, 255);

    SDL_Color white = {255, 255, 255, 255};

    TTF_Font* font = open_font(70);
    char str[18] = "score", tmp[12];
    sprintf(tmp, "   %d", score);
    strcat(str, tmp);
    draw_string(renderer, font, str, height / 4, width, true);
    TTF_CloseFont(font);

    SDL_RenderPresent(renderer);
}

char* enter_name(SDL_Renderer* renderer, int score, int h, int w) {
    bool quit = false, leftright = false, updown = false;
    char str[] = "AAA";
    int state = 0;
    SDL_Rect state_coord;
    TTF_Font* font = open_font(70);
    state_coord = draw_string(renderer, font, str, h / 2, w, true);
    draw_line(renderer, state_coord, state, false);


    while(!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        if(event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_LEFT:
                        draw_line(renderer, state_coord, state, true);
                        state -= 1;
                        if(state < 0)
                            state = 2;
                        draw_line(renderer, state_coord, state, false);
                        break;
                    case SDLK_RIGHT:
                        draw_line(renderer, state_coord, state, true);
                        state += 1;
                        if(state > 2)
                            state = 0;
                        draw_line(renderer, state_coord, state, false);
                        break;
                    case SDLK_UP:
                        delete_string(renderer, state_coord);
                        if(str[state] == 'A')
                            str[state] = '9';
                        else {
                            if(str[state] == '0')
                                str[state] = 'Z';
                            else
                                str[state] -= 1;
                        }

                        draw_string(renderer, font, str, h / 2, w, true);
                        break;
                    case SDLK_DOWN:;
                        delete_string(renderer, state_coord);
                        if(str[state] == 'Z')
                            str[state] = '0';
                        else {
                            if(str[state] == '9')
                                str[state] = 'A';
                            else
                                str[state] += 1;
                        }
                        draw_string(renderer, font, str, h / 2, w, true);
                        break;
                    case SDLK_RETURN:
                        quit = true;
                        break;
                    default:
                        break;
                }
        }
        if(event.type == SDL_QUIT)
            quit = true;

        SDL_RenderPresent(renderer);
    }
    TTF_CloseFont(font);
    char* record = (char*)malloc(15 * sizeof(char));
    strcpy(record, str);
    char tmp[12];
    sprintf(tmp, " %d", score);
    strcat(record, tmp);
    return record;
}

void draw_line(SDL_Renderer* renderer, SDL_Rect rect, int d, bool delete) {
    if(delete)
        boxRGBA(renderer, rect.x + (d * 40), rect.y + 35 + 25, rect.x + (d * 40) + 35, rect.y + 35 + 30, 0, 0, 0, 255);
    else
        boxRGBA(renderer, rect.x + (d * 40), rect.y + 35 + 25, rect.x + (d * 40) + 35, rect.y + 35 + 30, 255, 238, 0, 255);
}

void delete_string(SDL_Renderer* renderer, SDL_Rect rect) {
    boxRGBA(renderer, rect.x - 5, rect.y, rect.x + rect.w, rect.y + 55, 0, 0, 0, 255);
}

void record_to_file(char* record) {
    list* first = NULL;
    FILE* fp = fopen("ranking.txt", "r");
    if(fp == NULL) {
        fp = fopen("ranking.txt", "wt");
        fprintf(fp, "1. %s\n", record);
        return;
    }
    else {
        first = file_to_list(fp);
        first = insert_record(first, record);
        fclose(fp);
        if(remove("ranking.txt") != 0) {
            printf("Can't delete ranking\nexiting program.\n");
            exit(1);
        }
        fp = fopen("ranking.txt", "wt");
        list_to_file(fp, first);
    }

    fclose(fp);
    free(record);
}

list* file_to_list(FILE* fp) {
    list* first = NULL, *moving = NULL;
    int score, tmp;
    char name[3], buf[2];
    char str[100];

    /*while(fscanf(fp, "&s %s %d", buf, name, &score) != EOF) {
        first = new_list_element(first, name, score);
        printf("%s %s %d", buf, name, score);
    }*/
    while(fgets(str, 100, fp) != NULL) {
        sscanf(str, "%d. %s %d", &tmp, name, &score);
        if(first == NULL) {
            first = new_list_element(name, score);
            moving = first;
        }
        else {
            moving->next = new_list_element(name, score);
            moving = moving->next;
        }
    }

    return first;
}

list* new_list_element(char* name, int score) {
    list* tmp = (list*)malloc(sizeof(list));
    strcpy(tmp->name, name);
    tmp->score = score;
    tmp->next = NULL;
    return tmp;
}

list* insert_record(list* first, char* record) {
    int score;
    char name[3];
    sscanf(record, "%s %d", name, &score);
    list* moving = first, *late = first;
    list* tmp = NULL;
    if(score > first->score) {
        tmp = (list*)malloc(sizeof(list));
        strcpy(tmp->name, name);
        tmp->score = score;
        tmp->next = first;
        return tmp;
    }
    else {
        if(first->next == NULL) {
            first->next = new_list_element(name, score);
            return first;
        }
        for (int i = 0; i <= 10; ++i) {
            if(moving == NULL) {
                late->next = new_list_element(name, score);
                break;
            }
            if (score > moving->score) {
                tmp = new_list_element(name, score);
                late->next = tmp;
                tmp->next = moving;
                break;
            }
            late = moving;
            moving = moving->next;
        }
    }
    return first;
}

void list_to_file(FILE* fp, list* first) {
    list* moving = first;
    for(int i = 1; i <= 10; ++i) {
        fprintf(fp, "%d. %s %d\n", i, first->name, first->score);
        first = first->next;
        free(moving);
        if(first == NULL)
            return;
        moving = first;
    }
}

bool ranking_build(SDL_Renderer* renderer, int w, int h) {
    FILE *fp = fopen("ranking.txt", "rt");
    if (fp == NULL) {
        TTF_Font *font = open_font(70);
        draw_string(renderer, font, "no records yet.", h / 2, w, true);
        TTF_CloseFont(font);
    }
    else {
        char str[100];
        int d = h / 20, current_height = d - (d / 2);
        TTF_Font* font = open_font(d);
        list *first = file_to_list(fp), *moving = NULL;
        if(first == NULL)
            draw_string(renderer, font, "no records yet.", h / 2, w, true);
        for(int i = 0; i <= 10; ++i) {
            if(first == NULL)
                break;
            sprintf(str, "%d   %s %d", i + 1, first->name, first->score);
            draw_string(renderer, font, str, current_height, w, true);
            current_height += 2 * d;
            moving = first;
            first = first->next;
            free(moving);
        }
        TTF_CloseFont(font);
    }
    SDL_RenderPresent(renderer);
    bool quit = false;
    while(!quit) {
        SDL_Event event;
        SDL_WaitEvent(&event);

        if(event.type == SDL_KEYDOWN) {
            if(event.key.keysym.sym == SDLK_RETURN)
                quit = true;
        }
        if(event.type == SDL_QUIT)
            return true;
    }
    return false;
}