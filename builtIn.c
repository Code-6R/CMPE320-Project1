#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "builtIn.h"

int exitWish(char *arg[]) {
    if (arg[1] == NULL) {
        exit(EXIT_SUCCESS);
    } else {
        fprintf(stderr, "An error has occurred\n");
        exit(EXIT_FAILURE);
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

char *external(char *arg[], char **pathList) {
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
            program = pathListCopy[i];
            if (getpid() != parent) {
                execv(program, arg);
            }
        }
        i++;
    }
    if (program == NULL) {
        fprintf(stderr, "An error has occurred\n");
    }
    return program;
}
