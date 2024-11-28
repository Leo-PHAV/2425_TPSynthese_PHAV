#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "constantHeader.h"


void displayWelcomeMessage() {
    write(STDOUT_FILENO, welcomeMessage, strlen(welcomeMessage));
    write(STDOUT_FILENO, quitMessage, strlen(quitMessage));
}

void readCommand(){
    while (1) {
        write(STDOUT_FILENO, prompt, strlen(prompt));

        ssize_t bytesRead = read(STDIN_FILENO, buffer, sizeof(buffer) - 1);

         if (bytesRead == 0) {
            write(STDOUT_FILENO, exitMessage, strlen(exitMessage));
            break;
        }

        if (bytesRead > 0) {
            buffer[bytesRead - 1] = '\0'; 
            if (strcmp(buffer, exitCommand) == 0) {
                write(STDOUT_FILENO, exitMessage, strlen(exitMessage));
                break;
            }
        }

        pid_t pid = fork();
        if (pid == 0) {
            execlp(buffer, buffer, NULL);
            perror("Unknown command");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            wait(NULL);
        }
    }
}   




int main() {
    //displayWelcomeMessage();
    readCommand();
    return 0;
}