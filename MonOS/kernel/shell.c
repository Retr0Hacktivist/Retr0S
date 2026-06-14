#include "shell.h"
#include "screen.h"
#include "keyboard.h"
#include "fs.h"

#define MAX_CMD 256

static char cmd[MAX_CMD];
static int cmd_len = 0;

static void cmd_calc(char *args){
    int i = 0;
    int op_pos = 0;
while (args[i] != '\0') {
    if (args[i] == '+') {
        op_pos = i;  // ← sauvegarder ici
    }
    i++;
}
int a = args[0] - '0';        // premier nombre
int b = args[op_pos+1] - '0'; // deuxième nombre
int resultat = a + b;
print_char (resultat + '0' );
}


static void cmd_shutdown(void) {
    __asm__ volatile ("outw %0, %1" : : "a"((unsigned short)0x2000), "Nd"((unsigned short)0x604));
}

static void cmd_reboot(void) {
    // Envoyer la commande reboot au contrôleur clavier (port 0x64)
    __asm__ volatile ("outb %0, %1" : : "a"((unsigned char)0xFE), "Nd"((unsigned short)0x64));
}

static void cmd_neofetch(void) {
    print_string("\n");
    print_string("  ____      _        ___  \n");
    print_string(" |  _ \\ ___| |_ _ __/ _ \\ \n");
    print_string(" | |_) / _ \\ __| '__| | | |\n");
    print_string(" |  _ <  __/ |_| |  | |_| |\n");
    print_string(" |_| \\_\\___|\\__|_|   \\___/ \n");
    print_string("\n");
    print_string("  OS      : Retr0 v0.1\n");
    print_string("  Arch    : x86 32-bit\n");
    print_string("  Shell   : Retr0sh\n");
    print_string("  Auteur  : Retr0\n");
    print_string("\n");
}

static void cmd_clear(void) {
    clear_screen();
}

static void cmd_echo(char *args) {
    print_string(args);
    print_char('\n');
}

// Comparer deux strings
static int streq(const char *a, const char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

// Chercher le premier espace
static int starts_with(const char *cmd, const char *word) {
    int i = 0;
    while (word[i]) {
        if (cmd[i] != word[i]) return 0;
        i++;
    }
    return 1;
}

static void execute(void) {
    print_char('\n');

    if (cmd_len == 0) {
        // rien
    } else if (streq(cmd, "clear")) {
        cmd_clear();
    } else if (streq(cmd, "neofetch")) {
        cmd_neofetch();
    } else if (starts_with(cmd, "echo ")) {
        cmd_echo(cmd + 5);  // tout ce qui est après "echo "
    } else if (streq(cmd, "help")) {
    print_string("Commandes disponibles :\n");
    print_string("  clear\n");
    print_string("  echo <texte>\n");
    print_string("  neofetch\n");
    print_string("  calc\n");
    print_string("  ls\n");
    print_string("  touch\n");
    print_string("  reboot\n");
    print_string("  shutdown\n");
    print_string("  help\n");
    } else if (streq(cmd, "reboot")) {
        cmd_reboot();
} else if (starts_with(cmd, "calc ")) {
    cmd_calc(cmd + 5);
    } else if (streq(cmd, "shutdown")) {
        cmd_shutdown();
    
    } else if (streq(cmd, "ls")) {
    fs_ls();
    } else if (starts_with(cmd, "touch ")) {
        fs_touch(cmd + 6); 
    } else {
        print_string("Commande inconnue : ");
        print_string(cmd);
        print_char('\n');
    }

    // Reset
    cmd_len = 0;
    cmd[0] = '\0';
    print_string("> ");
}

void shell_run(void) {
    //clear_screen();
    print_string("> ");

    while (1) {
        char c = keyboard_getchar();

        if (c == '\n') {
            cmd[cmd_len] = '\0';
            execute();
        } else if (c == '\b') {
            if (cmd_len > 0) {
                cmd_len--;
                cmd[cmd_len] = '\0';
                print_char('\b');
            }
        } else if (cmd_len < MAX_CMD - 1) {
            cmd[cmd_len] = c;
            cmd_len++;
            print_char(c);
        }
    }
}
