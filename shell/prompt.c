#include "shell.h"

void sigchld_handler(int sig) {
    (void)sig;
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void set_prompt(char *prompt) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    struct passwd *pw = getpwuid(getuid());
    sprintf(prompt, "[%s@localhost:%s]$ ", pw->pw_name, cwd);
}

int parse_args(char *cmd, char **args) {
    int argc = 0;
    char *token = strtok(cmd, " \t\n");
    while (token && argc < MAX_ARG - 1) {
        args[argc++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[argc] = NULL;
    return argc;
}
