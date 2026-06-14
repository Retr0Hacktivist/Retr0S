#include "screen.h"
#include "idt.h"
#include "shell.h"

void kernel_main(void) {
    clear_screen();
    idt_init();

    print_string("  ____      _        ___  \n");
    print_string(" |  _ \\ ___| |_ _ __/ _ \\ \n");
    print_string(" | |_) / _ \\ __| '__| | | |\n");
    print_string(" |  _ <  __/ |_| |  | |_| |\n");
    print_string(" |_| \\_\\___|\\__|_|   \\___/ \n");
    print_string("\n");
    print_string("        Version 0.1\n");
    print_string("  Tape 'help' pour commencer\n");
    print_string("\n");

    shell_run();
}