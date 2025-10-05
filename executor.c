// executor.c — core fork/exec/wait logic for Lab 4

#include "executor.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * Returns:
 *   0..255  : child's exit code
 *   128+sig : if child terminated by a signal (like shells do)
 *   -1      : if fork()/waitpid() failed or unusual state
 */
int execute_command(char *command, char *args[]) {
    // Guard against empty/NULL input
    if (command == NULL || args == NULL || args[0] == NULL || args[0][0] == '\0') {
        return 0; // treat empty input as a no-op
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork failed");
        return -1;
    }

    if (pid == 0) {
        // --- CHILD ---
        execvp(command, args);

        // If we get here, exec failed
        perror("execvp");
        _exit(127); // conventional "command not found / exec failed"
    }

    // --- PARENT ---
    int status = 0;
    pid_t w = waitpid(pid, &status, 0);
    if (w == -1) {
        perror("waitpid failed");
        return -1;
    }

    if (WIFEXITED(status)) {
        return WEXITSTATUS(status);
    }
    if (WIFSIGNALED(status)) {
        return 128 + WTERMSIG(status);
    }

    // Stopped/continued/other unusual state
    return -1;
}

