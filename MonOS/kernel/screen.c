#include "screen.h"

#define VIDEO 0xB8000
#define COLS 80
#define ROWS 25

static char *video = (char *)VIDEO;
static int cursor = 0;

void print_char(char c) {
    if (c == '\n') {
        cursor += COLS - (cursor % COLS);
        return;
    }
    if (c == '\b') {
        if (cursor > 0) {
            cursor--;
            video[cursor * 2]     = ' ';
            video[cursor * 2 + 1] = 0x07;
        }
        return;
    }
    video[cursor * 2]     = c;
    video[cursor * 2 + 1] = 0x07;
    cursor++;
}

void print_string(const char *str) {
    int i = 0;
    while (str[i] != '\0') {
        print_char(str[i]);
        i++;
    }
}

void clear_screen(void) {
    int i;
    for (i = 0; i < COLS * ROWS; i++) {
        video[i * 2]     = ' ';
        video[i * 2 + 1] = 0x07;
    }
    cursor = 0;
}
