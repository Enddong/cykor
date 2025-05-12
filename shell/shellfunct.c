#include "shell.h"

int execute_single(char *cmd) {
    char tmp[1024];
    char *args[MAX_ARG];

    strncpy(tmp, cmd, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    char *redir = strchr(tmp, '>');
    int redirect = 0;
    char *outfile = NULL;

    if (redir) {
        *redir = '\0';
        redir++;
        while (*redir == ' ') redir++;
        outfile = strtok(redir, " \t\n");
        redirect = 1;
    }

    parse_args(tmp, args);
    int builtin_result = run_builtin(args);
    if (builtin_result >= 0) return builtin_result;

    pid_t pid = fork();
    if (pid == 0) {
        if (redirect && outfile) {
            int fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd < 0) {
                perror("open failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }
        execvp(args[0], args);
        perror("execvp failed");
        exit(1);
    } else {
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
}

int execute_pipeline(char *line) {
    char *cmds[MAX_ARG];
    int count = 0;
    char *cmd = strtok(line, "|");

    while (cmd && count < MAX_ARG - 1) {
        cmds[count++] = cmd;
        cmd = strtok(NULL, "|");
    }
    cmds[count] = NULL;

    int in_fd = 0, fd[2];

    for (int i = 0; i < count; i++) {
        pipe(fd);
        pid_t pid = fork();
        if (pid == 0) {
            dup2(in_fd, 0);
            if (i < count - 1)
                dup2(fd[1], 1);
            close(fd[0]);

            char tmp[1024];
            strncpy(tmp, cmds[i], sizeof(tmp) - 1);
            tmp[sizeof(tmp) - 1] = '\0';

            char *args[MAX_ARG];
            parse_args(tmp, args);
            execvp(args[0], args);
            perror("execvp failed");
            exit(1);
        } else {
            wait(NULL);
            close(fd[1]);
            in_fd = fd[0];
        }
    }

    return 0;
}

int execute_logical(char *line) {
    char *copy = strdup(line);
    char *ptr = copy;
    int last_status = 0;

    while (*ptr) {
        char *cmd = ptr;
        char *next = NULL;
        int is_and = 0, is_or = 0;

        while (*ptr) {
            if (strncmp(ptr, "&&", 2) == 0) {
                is_and = 1;
                *ptr = '\0';
                next = ptr + 2;
                break;
            } else if (strncmp(ptr, "||", 2) == 0) {
                is_or = 1;
                *ptr = '\0';
                next = ptr + 2;
                break;
            }
            ptr++;
        }

        while (*cmd == ' ') cmd++;
        char *end = cmd + strlen(cmd) - 1;
        while (end > cmd && (*end == ' ' || *end == '\n')) {
            *end = '\0';
            end--;
        }

        if (*cmd) {
            last_status = execute_single(cmd);
        }

        if (is_and && last_status != 0) break;
        if (is_or && last_status == 0) break;

        if (!next) break;
        ptr = next;
    }

    free(copy);
    return last_status;
}

int execute_sequential(char *line) {
    char *copy = strdup(line);
    char *rest = copy;
    char *cmd;
    int status = 0;

    while ((cmd = strsep(&rest, ";"))) {
        while (*cmd == ' ') cmd++;
        char *end = cmd + strlen(cmd) - 1;
        while (end > cmd && (*end == ' ' || *end == '\n')) {
            *end = '\0';
            end--;
        }
        if (*cmd) {
            execute_command(cmd);
        }
    }

    free(copy);
    return status;
}

int execute_background(char *line) {
    char *cmd = strtok(line, "&");
    while (cmd) {
        pid_t pid = fork();
        if (pid == 0) {
            setsid();
            execute_single(cmd);
            exit(0);
        } else {
            printf("[Background pid %d]\n", pid);
        }
        cmd = strtok(NULL, "&");
    }
    return 0;
}


