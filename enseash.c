#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "constantHeader.h"
#include "enseash.h"



int main() {
    //displayWelcomeMessage();

    while (1) {
        displayPrompt(promptMessage);

        if (readUserCommand(buffer, BUFSIZE) == 0) {
            write(STDOUT_FILENO, exitMessage, strlen(exitMessage));
            break;
        }

        if (strcmp(buffer, exitCommand) == 0) {
            write(STDOUT_FILENO, exitMessage, strlen(exitMessage));
            break;
        }

        executeCommand(buffer, promptMessage);
    }

    return 0;
}

void displayWelcomeMessage() {
    write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
}

void displayPrompt(const char *message) {
    write(STDOUT_FILENO, message, strlen(message));
}

ssize_t readUserCommand(char *buffer, size_t bufferSize) {
    ssize_t bytesRead = read(STDIN_FILENO, buffer, bufferSize - 1);

    if (bytesRead > 0) {
        buffer[bytesRead - 1] = '\0';
    }

    return bytesRead;
}

void executeCommand(const char *command, char *promptMessage) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        execlp(command, command, NULL);
        perror("Unknown command");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        wait(&status);

        if (WIFEXITED(status)) {
            sprintf(promptMessage, "enseash [exit:%d] %% ", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            sprintf(promptMessage, "enseash [sign:%d] %% ", WTERMSIG(status));
        } else {
            strcpy(promptMessage, "enseash %% ");
        }
    }
}