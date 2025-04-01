#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

#define COMMAND_DELIM " \n\r\t"

typedef struct {
    int symCnt;
    int dirSym;
    bool valid;
} sym_t;

void printErr();
int cd(char *arg[]);
char **path(char *arg[], int argNum, char **pathList);
int *external(char *arg[], char **pathList);
int exitWish(char *arg[]);
bool whiteSpace(char c);
#endif //BUILTIN_H