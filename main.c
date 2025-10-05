// main.c — Simple Command Executor (Lab 4)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "executor.h"

#define INPUT_BUFSIZE 1024
#define MAX_ARGS      64

// Trim leading/trailing whitespace in-place
static char *trim(char *s) {
    if (!s) return s;
    // trim leading
    while (*s && isspace((unsigned char)*s)) s++;
    // trim trailing
    char *end = s + strlen(s);
    while (end > s && isspace((unsigned char)end[-1])) --end;
    *end = '\0';
    return s;
}

// Parse input line into command + args (NULL-terminated). Modifies line.
static void parse_input(char *line, char **out_command, char *args[]) {
    *out_command = NULL;

    // strip trailing newline
    size_t len = strlen(line);
    if (len > 0 && line[len - 1] == '\n') line[len - 1] = '\0';

    // trim whitespace
    char *p = trim(line);
    if (*p == '\0') {           // empty line
        args[0] = NULL;
        return;
    }

    // Tokenize by whitespace
    int argc = 0;
    char *tok = strtok(p, " \t");
    while (tok && argc < MAX_ARGS - 1) {
        args[argc++] = tok;
        tok = strtok(NULL, " \t");
    }
    args[argc] = NULL;

    if (argc > 0) {
        *out_command = args[0];
    }
}

int main(void) {
    char line[INPUT_BUFSIZE];
    char *args[MAX_ARGS];
    char *command = NULL;

    printf("========================================\n");
    printf("   Simple Command Executor\n");
    printf("========================================\n");
    printf("Type 'exit' to quit\n\n");

    while (1) {
        // Prompt
        printf("cmd> ");
        fflush(stdout);

        // Read line (handle EOF / Ctrl-D)
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\nGoodbye!\n");
            break;
        }

        // Parse into command + args
        parse_input(line, &command, args);

        // Skip empty lines
        if (command == NULL || command[0] == '\0') {
            continue;
        }

        // Skip comment lines (#... or //...)
        if (command[0] == '#' || (command[0] == '/' && args[0][1] == '/')) {
            continue;
        }

        // Built-in: exit
        if (strcmp(command, "exit") == 0) {
            printf("Goodbye!\n");
            break;
        }

        // Execute external command
        int status = execute_command(command, args);
        printf("Command '%s' completed with exit status: %d\n\n", command, status);
    }

    return 0;
}

