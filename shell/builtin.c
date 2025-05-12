#include "shell.h"

int run_builtin(char **args) {
    if (!args[0]) return 1;

    if (strcmp(args[0], "exit") == 0) {
        exit(0);
    } else if (strcmp(args[0], "cd") == 0) {
        const char *path = args[1];
        if (!path || strcmp(path, "~") == 0) {
            path = getenv("HOME");
        }
        if (chdir(path) != 0) {
            perror("cd failed");
            return 1;
        }
        return 0;
    } else if (strcmp(args[0], "pwd") == 0) {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        printf("%s\n", cwd);
        return 0;
    }

    return -1;
}
