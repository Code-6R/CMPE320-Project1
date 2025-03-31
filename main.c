#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "builtIn.h"
#include <sys/wait.h>

int parHandle(char *arg[], char **argArr[], int arrCnt, char *argHolder[], int j) {
    if (strcmp(arg[j], "&") == 0 && j != 0) { // used for space case
        arg[j] = NULL;
        argArr[arrCnt] = &arg[j + 1];
        arrCnt++;
    }
    int k = 0;
    if (arrCnt == 0) {
        char *parSym = NULL;
        while ((parSym = strchr(arg[j], '&')) != NULL) { // used for no space case
            argHolder[k] = strndup(arg[j], strlen(arg[j]) - strlen(parSym));
            strsep(&parSym, "&");
            arg[j] = parSym;
            k++;
        }
        if (parSym == NULL && k != 0) {
            argHolder[k] = arg[j];
            for (arrCnt = 0; arrCnt <= k; arrCnt++)
                argArr[arrCnt] = &argHolder[arrCnt];
        }
    }
    if (arrCnt == 0 && k == 0) { // used for single command case
        argArr[0] = &arg[0];
        arrCnt = 1;
    }
    return arrCnt; // I thought this was better than making it a global variable
}

int main(int argc, char *argv[]) {
    int lpCnt = 1; // set to 1 since 0 contains the
    // user, device name, and current directory
    char *line = NULL;
    size_t size = 0;
    FILE *stream = NULL;
    char **pathList = malloc(sizeof(char) * 100);
    pathList[0] = "/bin";
    if (argc != 2 && argc != 1) {
        printErr();
        exit(EXIT_FAILURE);
    }
    if (argc == 1) {
        stream = stdin;
        printf("wish> ");
    }
    if (argc == 2) {
        stream = fopen(argv[1], "r");
    }
    if (stream == NULL) {
        printErr();
        exit(EXIT_FAILURE);
    }
    while (getline(&line, &size, stream) != -1) {
        if (argc == 1) {
            printf("wish> ");
        }
        while (strlen(line) && whiteSpace(*line)) {
            line++;
        }
        char *end = line + strlen(line) - 1;
        while (end > line && whiteSpace(*end)) end--;
        *(end + 1) = '\0';
        int j = 0;
        char *arg[sizeof(&line) + 1] = {NULL};
        char *argHolder[sizeof(&line) + 1] = {NULL};
        char **argArr[10] = {NULL}; // probably change the 10
        int arrCnt = 0;
        while ((arg[j] = strsep(&line, " \t")) != NULL) {
            arrCnt = parHandle(arg, argArr, arrCnt, argHolder, j);
            j++;
        }
        int arrIndex = 0;
        while (argArr[arrIndex] != NULL) {
            if (argArr[arrIndex][0] != NULL) {
                bool isExternal = true;
                int argNum = j;
                if (strcmp(argArr[arrIndex][0], "exit") == 0) {
                    isExternal = false;
                    exitWish(argArr[arrIndex]);
                }
                if (strcmp(argArr[arrIndex][0], "cd") == 0) {
                    isExternal = false;
                    cd(argArr[arrIndex]);
                }
                if (strcmp(argArr[arrIndex][0], "path") == 0) {
                    isExternal = false;
                    pathList = path(argArr[arrIndex], argNum, pathList);
                }
                if (isExternal == true && strcmp(argArr[arrIndex][0], "&") != 0) {
                    external(argArr[arrIndex], pathList);
                }
            }
            arrIndex++;
        }
        lpCnt++;
    }
}
