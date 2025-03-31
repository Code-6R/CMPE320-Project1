#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "builtIn.h"

int main(int argc, char *argv[]) {
    int lpCnt = 1; // set to 1 since 0 contains the
    // user, device name, and current directory
    char *line = NULL;
    size_t size = 0;
    FILE *stream = NULL;
    char **pathList = malloc(sizeof(char) * 100);
    pathList[0] = "/bin";
    if (argc != 2 && argc != 1) {
        fprintf(stderr, "An error has occurred\n");
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
        fprintf(stderr, "An error has occurred\n");
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
        char *argTemp[sizeof(&line) + 1] = {NULL};
        char **argArr[100] = {NULL}; // probably change the 100 later
        int arrCnt = 0;
        char *parSym = {NULL};
        while ((arg[j] = strsep(&line, " \t")) != NULL) {
            if (strcmp(arg[j], "&") == 0 && j != 0) {
                arg[j] = NULL;
                argArr[arrCnt] = &arg[j + 1];
                arrCnt++;
            }
            int k = 0;
            if (arrCnt == 0) {
                parSym = strchr(arg[j], '&');
                while ((parSym = strchr(arg[j], '&')) != NULL) {
                    argTemp[k] = strndup(arg[j], strlen(arg[j]) - strlen(parSym));
                    arg[j][*parSym] = '\0';
                    strsep(&parSym, "&");
                    arg[j] = parSym;
                    k++;
                }
                if (parSym == NULL && k != 0) {
                    argTemp[k] = arg[j];
                    for (arrCnt = 0; arrCnt <= k; arrCnt++)
                        argArr[arrCnt] = &argTemp[arrCnt];
                }
            }
            if (arrCnt == 0 && k == 0) {
                argArr[0] = &arg[0];
                arrCnt = 1;
            }
            j++;
        }
        int arrIndex = 0;
        while (argArr[arrIndex] != NULL) {
            if (argArr[arrIndex][0] != NULL) {
                int argNum = j;
                if (strcmp(argArr[arrIndex][0], "exit") == 0) {
                    exitWish(argArr[arrIndex]);
                }
                if (strcmp(argArr[arrIndex][0], "cd") == 0) {
                    cd(argArr[arrIndex]);
                }
                if (strcmp(argArr[arrIndex][0], "path") == 0) {
                    pathList = path(argArr[arrIndex], argNum, pathList);
                }
                if (strcmp(argArr[arrIndex][0], "cd") != 0
                    && strcmp(argArr[arrIndex][0], "path") != 0
                    && strcmp(argArr[arrIndex][0], "exit") != 0
                    && strcmp(argArr[arrIndex][0], "&") != 0) {
                    external(argArr[arrIndex], pathList);
                    }
            }
            arrIndex++;
        }
      lpCnt++;
    }
}

