#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <readline/readline.h>
#include <readline/history.h>

#define MAX_ARG 100
#define MAX_CMD 1024

void set_prompt(char *prompt);
void execute_command(char *cmd);
void history_setup();
void history_finish();
int run_builtin(char **args);
int execute_single(char *cmd);
int execute_pipeline(char *line);
int execute_logical(char *line);
int execute_background(char *line);
int execute_sequential(char *line);

void set_prompt(char *prompt) {
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    struct passwd *pw = getpwuid(getuid());
    sprintf(prompt, "[%s@localhost:%s]$ ", pw->pw_name, cwd);
}

void history_setup() {
    using_history();
    stifle_history(50);
    read_history("/tmp/msh_history");
}

void history_finish() {
    append_history(history_length, "/tmp/msh_history");
    history_truncate_file("/tmp/msh_history", 50);
}

void execute_command(char *line) {
    if (strchr(line, ';')) {
        execute_sequential(line);
    } else if (strstr(line, "&&") || strstr(line, "||")) {
        execute_logical(line);
    } else if (strchr(line, '|')) {
        execute_pipeline(line);
    } else if (strchr(line, '&')) {
        execute_background(line);
    } else {
        execute_single(line);
    }
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

int execute_single(char *cmd) {
    char tmp[1024];
    char *args[MAX_ARG];
    strcpy(tmp, cmd);

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
            strcpy(tmp, cmds[i]);
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

int main() {
    char prompt[1024];
    char *line;

    history_setup();

    while (1) {
        set_prompt(prompt);
        line = readline(prompt);
        if (!line) break;
        if (*line) add_history(line);
        execute_command(line);
        free(line);
    }

    history_finish();
    return 0;
}

