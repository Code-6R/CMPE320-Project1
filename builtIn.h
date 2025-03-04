#ifndef BUILTIN_H
#define BUILTIN_H
int cd(char *arg[]);
char **path(char *arg[], int argNum, char **pathList);
void printPath(char **pathList, int argNum);
char *external(char *arg[], char **pathList);
int exitWish(char *arg[]);
int redirect(char *arg[]);
#endif //BUILTIN_H
