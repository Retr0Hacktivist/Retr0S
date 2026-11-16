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
// Comparer deux strings (pareil que dans shell.c)
static int streq(const char *a, const char *b) {
    int i = 0;
    while (a[i] && b[i]) {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return a[i] == b[i];
}

void fs_touch(const char *nom) {
    int i;
    int j;

    // vérifier que le nom rentre dans nom[32] (max 31 caractères)
    for (j = 0; nom[j] != '\0'; j++) {
        if (j >= 31) {
            print_string("Nom trop long (max 31)\n");
            return;
        }
    }

    // vérifier si le fichier existe déjà
    for (i = 0; i < 16; i++) {
        if (fs[i].nom[0] != '\0' && streq(fs[i].nom, nom)) {
            print_string("Fichier deja existant\n");
            return;
        }
    }

    // chercher un slot libre
    for (i = 0; i < 16; i++) {
        if (fs[i].nom[0] == '\0') {  // si le slot est libre
            for (j = 0; nom[j] != '\0'; j++) {
                fs[i].nom[j] = nom[j];
            }
            fs[i].nom[j] = '\0';
            fs[i].taille = 0;
            nb_fichiers++;
            return;
        }
    }

    // aucun slot libre
    print_string("Disque plein\n");
}