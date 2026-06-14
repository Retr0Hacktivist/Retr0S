#include "fs.h"
#include "screen.h"

struct fichier fs[16];  // le "disque dur"
int nb_fichiers = 0;

void fs_init(void) {
    // vider tous les slots au démarrage
    int i;
    for (i = 0; i < 16; i++) {
        fs[i].nom[0] = '\0';
        fs[i].taille = 0;
    }
}
void fs_ls(void) {
    int i;
    for (i = 0; i < 16; i++) {
        if (fs[i].nom[0] != '\0') {  // si le fichier existe
            print_string(fs[i].nom);
            print_char('\n');
        }
    }
}
void fs_touch(const char *nom) {
    int i;
    for (i = 0; i < 16; i++) {
        if (fs[i].nom[0] == '\0') {  // si le fichier existe
            int j = 0;
        while (nom[j] != '\0') {
        fs[i].nom[j] = nom[j];
        j++;
            }
fs[i].nom[j] = '\0';}
break;
    }
}