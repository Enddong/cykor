#include "shell.h"

int main() {
    char prompt[1024];
    char *line;

    signal(SIGCHLD, sigchld_handler);

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
