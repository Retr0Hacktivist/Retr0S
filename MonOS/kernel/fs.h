#ifndef FS_H
#define FS_H

struct fichier {
    char nom[32];
    char contenu[512];
    int taille;
};

void fs_init(void);
void fs_ls(void);
void fs_touch(const char *nom);
void fs_cat(const char *nom);
void fs_write(const char *nom, const char *contenu);

#endif
