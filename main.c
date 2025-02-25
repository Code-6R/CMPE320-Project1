#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
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
    while (getline(&line, &size, stream) != -1) {
      //  printf("line%d: %s", i, line);
        if (argc == 1) {
            printf("wish> ");
        }
        int j = 0;
        line[strlen(line) - 1] = '\0';
        char *arg[sizeof(&line)];
        while ((arg[j] = strsep(&line, " ")) != NULL) {
            j++;
        }
        int argNum = j;
        if (strcmp(arg[0], "exit") == 0) {
            exitWish(arg);
        }
        if (strcmp(arg[0], "cd") == 0) {
            cd(arg);
        }
        if (strcmp(arg[0], "path") == 0) {
            pathList = path(arg, argNum, pathList);
        }
        // printPath(pathList, argNum); // testing if pathList updates properly
        if (strcmp(arg[0], "cd") !=0 && strcmp(arg[0], "path") != 0
            && strcmp(arg[0], "exit") != 0) {
            char *executable = external(arg, pathList);
        }
      lpCnt++;
    }
    free(pathList);
    free(line);
    fclose(stream);
}

