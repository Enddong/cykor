#ifndef SHELL_H
#define SHELL_H

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
#include <signal.h>

#define MAX_ARG 100
#define MAX_CMD 1024

void sigchld_handler(int sig);
void set_prompt(char *prompt);
void execute_command(char *cmd);
void history_setup();
void history_finish();
int run_builtin(char **args);
int parse_args(char *cmd, char **args);

int execute_single(char *cmd);
int execute_pipeline(char *line);
int execute_logical(char *line);
int execute_background(char *line);
int execute_sequential(char *line);

#endif
