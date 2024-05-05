#include "file.h"

int** read_map() {
    FILE* fp = fopen("map.txt", "rt");
    if(fp == NULL) {
        puts("Can't open map\nexiting program\n");
        exit(1);
    }

    LIST* first = NULL, *moving = NULL;

    int y = 0, x = 0, xmax = 0;
    char c;
    while((c = (char)fgetc(fp)) != EOF) {
        if (isalnum(c)) {
            if (first == NULL) {
                moving = new_element(x, y, c);
                first = moving;
            } else {
                moving->next = new_element(x, y, c);
                moving = moving->next;
            }
            ++x;
        }
        else if(c == '\n') {
            if(y == 0)
                xmax = x;
            x = 0;
            ++y;
        }
    }
    int** map = list_to_array(first, xmax, y);
    return map;
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

LIST* new_element(int x, int y, char value) {
    LIST* new = (LIST*)malloc(sizeof(LIST));
    new->x = x;
    new->y = y;
    new->value = value;
    new->next = NULL;

    return new;
}

/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/

int** list_to_array(LIST* plist, int x, int y) {
    LIST* tmp;

    int** map;
    map = (int**)malloc(y * sizeof(int*));
    for(int i = 0; i <= y; ++i) {
        map[i] = (int*) malloc(x * sizeof(int));
    }

    while(plist != NULL) {
        map[plist->y][plist->x] = plist->value - '0';
        tmp = plist;
        plist = plist->next;
        free(tmp);
    }

    return map;
}
