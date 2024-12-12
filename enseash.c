#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <fcntl.h>


#include "constantHeader.h"
#include "enseash.h"




int main() {
    char promptMessage[BUFSIZE] = "enseash % ";
    char buffer[BUFSIZE] = {0};

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

void displayPrompt(const char *promptMessage) {
    write(STDOUT_FILENO, promptMessage, strlen(promptMessage));
}

ssize_t readUserCommand(char *buffer, size_t bufferSize) {
    ssize_t bytesRead = read(STDIN_FILENO, buffer, bufferSize - 1);

    if (bytesRead > 0) {
        buffer[bytesRead - 1] = '\0';
    }

    return bytesRead;
}

int calculateElapsedTime(const struct timespec *start, const struct timespec *end) {
    return (end->tv_sec - start->tv_sec) * 10000 +
           (end->tv_nsec - start->tv_nsec) / 1e6;
}

void complexCommand(const char *buf, char **args, size_t maxArgs) {
    size_t i = 0;

    char *command = strdup(buf);
    char *token = strtok(command, " ");

    while (token != NULL && i < maxArgs - 1) {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;
}

int redirections(char **args) {
    int fd;
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], "<") == 0) {
            fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("Error opening input file");
                return -1;
            }
            dup2(fd, STDIN_FILENO);
            close(fd);

            args[i] = NULL;
            args[i + 1] = NULL;
        } else if (strcmp(args[i], ">") == 0) {
            fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Error opening output file");
                return -1;
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);

            args[i] = NULL;
            args[i + 1] = NULL;
        }
    }

    return 0;
}

void executeCommand(const char *buf, char *promptMessage) {
    pid_t pid;
    struct timespec startTime, endTime;

    clock_gettime(CLOCK_MONOTONIC, &startTime);

    char *args[BUFSIZE];
    
    complexCommand(buf, args, BUFSIZE);

    pid = fork();
    if (pid == 0) {
        if (redirections(args) < 0) {
            exit(EXIT_FAILURE);
        }
        execvp(args[0], args);
        perror("Unknown command");
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        int status;
        wait(&status);

        clock_gettime(CLOCK_MONOTONIC, &endTime);
        int elapsedTime = calculateElapsedTime(&startTime, &endTime);

        if (WIFEXITED(status)) {
            sprintf(promptMessage, "enseash [exit:%d|%dms] %% ", WEXITSTATUS(status), elapsedTime);
        } else if (WIFSIGNALED(status)) {
            sprintf(promptMessage, "enseash [sign:%d|%dms] %% ", WTERMSIG(status), elapsedTime);
        } else {
            strcpy(promptMessage, "enseash %% ");
        }
    }

    free(args[0]);
}

