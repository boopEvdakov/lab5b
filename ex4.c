#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_INPUT 256
#define MAX_ARGS 64

int main() {
    char input[MAX_INPUT];
    char *args[MAX_ARGS];

    while (1) {
        printf("myshell> ");
        fflush(stdout);

        if (fgets(input, MAX_INPUT, stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            continue;
        }

        int argc = 0;
        char *token = strtok(input, " \t");
        while (token != NULL && argc < MAX_ARGS - 1) {
            args[argc++] = token;
            token = strtok(NULL, " \t");
        }
        args[argc] = NULL;

        if (argc == 0) {
            continue;
        }

        if (strcmp(args[0], "exit") == 0) {
            break;
        }

        char path[MAX_INPUT];
        const char *dirs[] = {"/bin/", "/usr/bin/", "/usr/local/bin/", NULL};
        int found = 0;

        if (args[0][0] == '/') {
            strcpy(path, args[0]);
            found = 1;
        } else {
            for (int i = 0; dirs[i] != NULL; i++) {
                snprintf(path, MAX_INPUT, "%s%s", dirs[i], args[0]);
                if (access(path, X_OK) == 0) {
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            printf("Command not found: %s\n", args[0]);
            continue;
        }

        pid_t pid = fork();

        if (pid < 0) {
            perror("fork failed");
            continue;
        }

        if (pid == 0) {
            execve(path, args, NULL);
            perror("execve failed");
            exit(1);
        } else {
            printf("[%d] Process started in background\n", pid);
        }
    }

    return 0;
}
