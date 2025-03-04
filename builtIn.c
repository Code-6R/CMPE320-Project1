#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "builtIn.h"
#include "sys/wait.h"

char **pathlist, *line;
FILE *stream;

int exitWish(char *arg[]) {
    if (arg[1] == NULL) {
        free(pathlist);
        free(line);
        free(stream);
        exit(EXIT_SUCCESS);
    } else {
        fprintf(stderr, "An error has occurred\n");
        return EXIT_FAILURE;
    }
}

int cd(char *arg[]) {
    char dir[1024] = {0};
    if (arg[2] != NULL || arg[1] == NULL) {
        fprintf(stderr, "An error has occurred\n");
    }
    if (arg[1] != NULL) {
        chdir(arg[1]);
    }
    getcwd(dir, sizeof(dir));
    // printf("%s\n", dir); // used to check current working directory
    return 0;
}

void printPath(char **pathList, int argNum) {
    if (argNum == 1) {
        printf("Path list is empty.\n");
        return;
    }
    printf("Current Path List:\n");
    int i = 0;
    while (pathList[i] != NULL ) {
        printf("%s\n", pathList[i]);
        i++;
    }
}

char **path(char *arg[], int argNum, char **pathList) {
    free(pathList);
    int i = 0;
    unsigned long argLen = 0;
    while (arg[i] != NULL) {
        argLen += strlen(arg[i]);
        i++;
    }
    pathList = malloc(sizeof(char) * argLen);
    for (int j = 0; j < argNum - 1; j++) {
        pathList[j] = strdup(arg[j + 1]);
    }
    if (argNum == 1) {
        pathList[0] = NULL;
    }
    // printPath(pathList, argNum); // testing if pathList updates properly
    return pathList;
}

sym_t redirect(char *arg[]) {
    sym_t sym;
    int i = 0;
    sym.dirSym = 0;
    sym.symCnt = 0;
    while (arg[i] != NULL) {
        if (strcmp(arg[i], ">") == 0) {
            sym.dirSym = i;

            sym.symCnt++;
        }
        i++;
    }
    if (sym.dirSym != 0 && arg[sym.dirSym + 1] != NULL
        && arg[sym.dirSym + 2] == NULL && sym.symCnt == 1) {
       freopen(arg[sym.dirSym + 1], "w+", stdout);
    }
    else if (sym.dirSym != 0 || sym.symCnt > 1) {
        fprintf(stderr, "An error has occurred\n");
    }
    return sym;
}

char *external(char *arg[], char **pathList) {
    int i = 0;
    char *pathListCopy[100];
    char *program = NULL;
    char *argCpy[100] = {NULL};
    while (pathList[i] != NULL && pathList[i][0] != '\0') {
        pathListCopy[i] = strdup(pathList[i]);
        strcat(pathListCopy[i], "/");
        strcat(pathListCopy[i], arg[0]);
        if (access(pathListCopy[i], X_OK) != -1) {
            int parent = getpid();
            fork();
            wait(NULL);
            program = pathListCopy[i];
            int j = 0;
            while (arg[j] != NULL && strcmp(arg[j], ">") != 0) {
                argCpy[j] = arg[j];
                j++;
            }
            if (getpid() != parent) {
                sym_t sym = redirect(arg);
                if (sym.dirSym != 0 && arg[sym.dirSym + 1] != NULL
                    && arg[sym.dirSym + 2] == NULL &&
                    sym.symCnt == 1|| sym.dirSym == 0) {
                    execv(program, argCpy);
                }
            }
        }
        i++;
    }
    if (program == NULL) {
        fprintf(stderr, "An error has occurred\n");
    }
    return program;
}
