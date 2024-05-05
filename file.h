#ifndef UNTITLED_FILE_H
#define UNTITLED_FILE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


typedef struct file_list {
    int x, y;
    char value;
    struct file_list* next;
} LIST;

int** read_map();
LIST* new_element(int x, int y, char value);
int** list_to_array(LIST* plist, int x, int y);
char** list_to_map(LIST* first);
char* dynamic_line(char* ptr, int size, char c);
char** dynamic_column(char** ptr, int size);
char** read_line(FILE* fp, int index);

#endif //UNTITLED_FILE_H
