#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "builtIn.h"
#include "sys/wait.h"
#include <fcntl.h>

char **pathlist, *line;
FILE *stream;

void printErr() {
    fprintf(stderr, "An error has occurred\n");
}

int exitWish(char *arg[]) {
    if (arg[1] == NULL) {
        free(pathlist);
        free(line);
        free(stream);
        exit(EXIT_SUCCESS);
    } else {
        printErr();
        return EXIT_FAILURE;
    }
}

int cd(char *arg[]) {
    char dir[1024] = {0};
    if (arg[2] != NULL || arg[1] == NULL) {
        printErr();
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
    return pathList;
}

bool whiteSpace(char c) {
    char *delims = COMMAND_DELIM;
    size_t len = strlen(COMMAND_DELIM);
    for (int i = 0; i < len; i++) {
        if (c == delims[i]) {
            return true;
        }
    }
    return false;
}

sym_t redirect(char *arg[], char *argCpy[]) {
    sym_t sym;
    int i = 0;
    int cnt = 0;
    sym.dirSym = 0;
    sym.symCnt = 0;
    sym.valid = 0;
    char *dest = {NULL};
    while (arg[i] != NULL) {
        if (strcmp(arg[i], ">") == 0) {
            sym.dirSym = i;
            sym.symCnt++;
            dest = arg[sym.dirSym + 1];
        }
        else if (sym.dirSym == 0) {
            dest = strchr(arg[i], '>');
            if (dest != NULL && i > 0) {
                arg[i][*dest] = '\0';
                cnt++;
            }
            strsep(&dest, ">");
        }
        if (strcmp(arg[i], ">") != 0) {
            argCpy[i] = arg[i];
        }
        if (strcmp(arg[i], "") == 0 && cnt == 1) {
            argCpy[i] = NULL;
        }
        i++;
    }
    if (sym.symCnt == 1 && arg[sym.dirSym + 1] != NULL
        && arg[sym.dirSym + 2] == NULL || cnt == 1) {
        sym.valid = 1;
        int output = open(dest, O_CREAT | O_TRUNC | O_WRONLY);
        dup2(output, fileno(stdout));
    }
    else if (sym.dirSym != 0 || sym.symCnt > 1) {
        printErr();
    }
    return sym;
}

void run(char *program, char *arg[]) {
    char *argCpy[100] = {NULL};
    sym_t sym = redirect(arg, argCpy);
    if (sym.valid == 1 || sym.dirSym == 0) {
        execv(program, argCpy);
    }
}

int *external(char *arg[], char **pathList) {
    int i = 0;
    char *pathListCopy[100];
    char *program = NULL;
    while (pathList[i] != NULL && pathList[i][0] != '\0') {
        pathListCopy[i] = strdup(pathList[i]);
        strcat(pathListCopy[i], "/");
        strcat(pathListCopy[i], arg[0]);
        if (access(pathListCopy[i], X_OK) != -1) {
            int parent = getpid();
            fork();
            wait(NULL);
            program = pathListCopy[i];
            if (getpid() != parent) {
                run(program, arg);
                exit(EXIT_SUCCESS);
            }
        }
        i++;
    }
    if (program == NULL) {
        printErr();
    }
    return 0;
}
