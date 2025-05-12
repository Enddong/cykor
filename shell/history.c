#include "shell.h"

void history_setup() {
    using_history();
    stifle_history(50);
    read_history("/tmp/msh_history");
}

void history_finish() {
    append_history(history_length, "/tmp/msh_history");
    history_truncate_file("/tmp/msh_history", 50);
}
