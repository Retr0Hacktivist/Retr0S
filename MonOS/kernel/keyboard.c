#include "keyboard.h"
#include "screen.h"

#define KEYBOARD_PORT 0x60
#define SCANCODE_MAP_SIZE 58

static char scancode_map[] = {
    0,   0,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0',
    '-', '=', '\b',   0,  'a', 'z', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '^', '$', '\n', 0, 'q', 's', 'd', 'f', 'g', 'h',
    'j', 'k', 'l', 'm', '%', '`', 0,  '*', 'w', 'x', 'c', 'v',
    'b', 'n', ',', ';', ':', '+', 0,   0,   0,  ' '
};

static unsigned char inb(unsigned short port) {
    unsigned char result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static void outb(unsigned short port, unsigned char val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static char key_buffer[256];
static int buf_start = 0;
static int buf_end   = 0;

void keyboard_handler_c(void) {
    unsigned char scancode = inb(KEYBOARD_PORT);

    if (!(scancode & 0x80)) {
        if (scancode < SCANCODE_MAP_SIZE) {
            char c = scancode_map[scancode];
            if (c != 0) {
                key_buffer[buf_end % 256] = c;
                buf_end++;
            }
        }
    }
    outb(0x20, 0x20);
}

char keyboard_getchar(void) {
    while (buf_start == buf_end);
    char c = key_buffer[buf_start % 256];
    buf_start++;
    return c;
}
