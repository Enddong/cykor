#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define TRUE 1
#define FALSE 0

char lastdir[100];
char command[BUFSIZ];
char argv[100][100];
char **argvtmp1;
char argv_redirect[100];
int argc;
int BUILTIN_COMMAND = 0;
int PIPE_COMMAND = 0;
int REDIRECT_COMMAND = 0;
int BACKGROUND = 0;

void set_prompt(char *prompt);
void builtin_command();
void execute_command(char **args, int input_fd);
char **parse_arguments(char *cmd);
void free_arguments(char **args);
void parse_and_execute(char *line);
void initial();
void init_lastdir();
void history_setup();
void history_finish();

int main() {
	char prompt[BUFSIZ];
	char *line;
	init_lastdir();
	history_setup();
	while (1) {
		set_prompt(prompt);
		if (!(line = readline(prompt))) break;
		if (*line) add_history(line);
		parse_and_execute(line);
		free(line);
	}
	history_finish();
	return 0;
}

void set_prompt(char *prompt) {
	char cwd[100];
	struct passwd* pwp;
	pwp = getpwuid(getuid());
	getcwd(cwd, sizeof(cwd));
	sprintf(prompt, "[%s@%s:%s]$ ", pwp->pw_name, "localhost", cwd);
}

void parse_and_execute(char *line) {
	char *token;
	char *saveptr;
	char *sep = line;
	int exec_next = 1;
	while ((token = strsep(&sep, ";")) != NULL) {
		char *cmd = token;
		if (strstr(cmd, "&&") || strstr(cmd, "||")) {
			char *sub_saveptr;
			char *cond_cmd = strtok_r(cmd, "&&||", &sub_saveptr);
			char *next_cmd = strtok_r(NULL, "", &sub_saveptr);
			parse_and_execute(cond_cmd);
			int status;
			wait(&status);
			if ((strstr(cmd, "&&") && WIFEXITED(status) && WEXITSTATUS(status) == 0) ||
				(strstr(cmd, "||") && (!WIFEXITED(status) || WEXITSTATUS(status) != 0))) {
				parse_and_execute(next_cmd);
			}
			continue;
		}
		if (strchr(cmd, '&')) {
			BACKGROUND = 1;
			cmd[strlen(cmd)-1] = '\0';
		}
		char *pipe_save;
		char *segment = strtok_r(cmd, "|", &pipe_save);
		int input_fd = 0;
		while (segment) {
			char *seg_copy = strdup(segment);
			char **args = parse_arguments(seg_copy);
			execute_command(args, input_fd);
			free(seg_copy);
			free_arguments(args);
			segment = strtok_r(NULL, "|", &pipe_save);
			if (segment) {
				int pipefd[2];
				pipe(pipefd);
				input_fd = pipefd[0];
				dup2(pipefd[1], STDOUT_FILENO);
				close(pipefd[1]);
			}
		}
		BACKGROUND = 0;
	}
}

char **parse_arguments(char *cmd) {
	char **args = malloc(sizeof(char*) * 100);
	int i = 0;
	char *token = strtok(cmd, " ");
	while (token != NULL) {
		if (strcmp(token, ">") == 0) {
			token = strtok(NULL, " ");
			if (token) strcpy(argv_redirect, token);
			REDIRECT_COMMAND = 1;
			break;
		}
		args[i++] = strdup(token);
		token = strtok(NULL, " ");
	}
	args[i] = NULL;
	return args;
}

void execute_command(char **args, int input_fd) {
	if (!args || !args[0]) return;
	if (strcmp(args[0], "exit") == 0) {
		exit(EXIT_SUCCESS);
	} else if (strcmp(args[0], "cd") == 0) {
		struct passwd* pwp;
		char cd_path[100];
		if (!args[1]) {
			pwp = getpwuid(getuid());
			sprintf(cd_path,"/home/%s",pwp->pw_name);
			chdir(cd_path);
		} else if (strcmp(args[1], "~") == 0) {
			pwp = getpwuid(getuid());
			sprintf(cd_path,"/home/%s",pwp->pw_name);
			chdir(cd_path);
		} else {
			chdir(args[1]);
		}
		return;
	}
	pid_t pid = fork();
	if (pid == 0) {
		if (input_fd != 0) {
			dup2(input_fd, 0);
			close(input_fd);
		}
		if (REDIRECT_COMMAND) {
			int fd = open(argv_redirect, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		execvp(args[0], args);
		perror("exec failed");
		exit(1);
	} else {
		if (!BACKGROUND) waitpid(pid, NULL, 0);
	}
	REDIRECT_COMMAND = 0;
}

void free_arguments(char **args) {
	for (int i = 0; args[i]; i++) free(args[i]);
	free(args);
}

void initial() {
	for(int i = 0; i < argc; i++) strcpy(argv[i], "\0");
	argc = 0;
	BUILTIN_COMMAND = 0;
	PIPE_COMMAND = 0;
	REDIRECT_COMMAND = 0;
	BACKGROUND = 0;
}

void init_lastdir() {
	getcwd(lastdir, sizeof(lastdir));
}

void history_setup() {
	using_history();
	stifle_history(50);
	read_history("/tmp/msh_history");
}

void history_finish() {
	append_history(history_length, "/tmp/msh_history");
	history_truncate_file("/tmp/msh_history", history_max_entries);
}
