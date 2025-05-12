#include "shell.h"

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
