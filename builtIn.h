#ifndef BUILTIN_H
#define BUILTIN_H

typedef struct {
    int symCnt;
    int dirSym;
} sym_t;

int cd(char *arg[]);
char **path(char *arg[], int argNum, char **pathList);
void printPath(char **pathList, int argNum);
char *external(char *arg[], char **pathList);
int exitWish(char *arg[]);
sym_t redirect(char *arg[]);
#endif //BUILTIN_H
